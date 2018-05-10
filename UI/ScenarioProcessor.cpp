#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include "ScenarioProcessor.h"
#include <QStandardPaths>
#include <QDir>
#include <QRegularExpressionMatch>

ScenarioProcessor::ScenarioProcessor(SiteConfig& siteConfig, PointSourceRupture &rupture, GMPE &gmpe,
                                     IntensityMeasure &intensityMeasure, RecordSelectionConfig &selectionConfig, SiteResult& siteResult,
                                     QObject *parent) : QObject(parent), m_siteConfig(siteConfig), m_rupture(rupture),
                                     m_gmpe(gmpe), m_intensityMeasure(intensityMeasure), m_selectionConfig(selectionConfig), m_siteResult(siteResult)
{
    //Init the working directory that will be used by this processor
    QString dataDirectory = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::GenericDataLocation);
    m_workDir = QDir::cleanPath(dataDirectory + QDir::separator() + "SimCenter-EQSS");
    if(!QDir(m_workDir).exists())
        QDir().mkdir(m_workDir);

    setupConnections();
}

SiteResultsModel &ScenarioProcessor::getResultsModel()
{
    return m_resultsModel;
}

void ScenarioProcessor::killProcesses()
{
    if(m_hazardAnalysisProcess.state() >= QProcess::ProcessState::Starting)
    {
        m_hazardAnalysisProcess.kill();
        m_hazardAnalysisProcess.waitForFinished();
    }

    if(m_simulationProcess.state() >= QProcess::ProcessState::Starting)
    {
        m_simulationProcess.kill();
        m_simulationProcess.waitForFinished();
    }

    if(m_recordSelectionProcess.state() >= QProcess::ProcessState::Starting)
    {
        m_recordSelectionProcess.kill();
        m_recordSelectionProcess.waitForFinished();
    }
}

void ScenarioProcessor::startProcessingScenario()
{
    emit statusUpdated("Started processing scenario");
    startHazardAnalysis();
}

void ScenarioProcessor::startHazardAnalysis()
{
    emit statusUpdated("Processing seismic hazard analysis...");

    QJsonObject scenarioInput;
    scenarioInput.insert("Site", m_siteConfig.getJson());
    scenarioInput.insert("EqRupture", m_rupture.getJson());
    scenarioInput.insert("GMPE", m_gmpe.getJson());
    scenarioInput.insert("IntensityMeasure", m_intensityMeasure.getJson());

    QJsonDocument inputDoc(scenarioInput);
    QFile inputFile(getWorkFilePath("Scenario.json"));
    if (!inputFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't write scenario input file.");
        return;
    }
    inputFile.write(inputDoc.toJson());
    inputFile.close();

    //TODO: This will need to be configured
    QString eqScenarioPath = "C:/SourceTree/Simcenter-EQSS/EQScenario/build/EQScenario.jar";

    QString inputName = getWorkFilePath("Scenario.json");
    QString outputName = getWorkFilePath("Scenario_" + m_intensityMeasure.type()+ ".json");
    QStringList args;
    args << "-jar" << eqScenarioPath << inputName << outputName;

    m_hazardAnalysisProcess.start("java", args);

    if(!m_hazardAnalysisProcess.waitForStarted(-1))
    {
        emit statusUpdated("Failed to start seismic hazard analysis!");
        return;
    }
}

void ScenarioProcessor::startSimulation()
{
    emit statusUpdated("Simulating intensity measure...");

    //First we need to check hazard analysis did not fail
    if(m_hazardAnalysisProcess.exitStatus() == QProcess::ExitStatus::CrashExit || m_hazardAnalysisProcess.exitCode() != 0)
        return;
    QString simulateIMPath = "C:/SourceTree/Simcenter-EQSS/build/SimulateGM/Release/SimulateGM.exe";


    QString hazardOutputPath = getWorkFilePath("Scenario_" + m_intensityMeasure.type()+ ".json");
    QString simConfigPath = getWorkFilePath("SimConfig.json");
    QString simOutputPath = getWorkFilePath("SimOutput.json");

    QJsonObject gmConfig;
    gmConfig.insert("File", hazardOutputPath);

    QJsonObject simulationConfig;
    simulationConfig.insert("GroundMotions", gmConfig);
    simulationConfig.insert("NumSimulations", 1);
    simulationConfig.insert("SpatialCorrelation", true);

    QJsonDocument simConfigDoc(simulationConfig);
    QFile simConfigFile(simConfigPath);
    if (!simConfigFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't write simulation config file.");
        return;
    }
    simConfigFile.write(simConfigDoc.toJson());
    simConfigFile.close();


    QStringList args;
    args << simConfigPath << simOutputPath;
    m_simulationProcess.start(simulateIMPath, args);

    if(!m_simulationProcess.waitForStarted(-1))
    {
        emit statusUpdated("Failed to start ground motion simulation!");
        return;
    }

}

void ScenarioProcessor::startSelection()
{
    emit statusUpdated("Selecting and scaling ground motion...");

    //First we need to check IM simulation
    if(m_simulationProcess.exitStatus() == QProcess::ExitStatus::CrashExit || m_simulationProcess.exitCode() != 0)
        return;
    QString selectRecordPath = "C:/SourceTree/Simcenter-EQSS/build/SelectGM/Release/SelectGM.exe";


    QString selectionConfigPath = getWorkFilePath("SelectionConfig.json");
    QString simOutputPath = getWorkFilePath("SimOutput.json");
    QString selectionOutputPath = getWorkFilePath("SelectionOutput.json");


    QJsonObject selectionConfig = m_selectionConfig.getJson();
    QJsonObject target = selectionConfig["Target"].toObject();
    target.insert("File", simOutputPath);
    selectionConfig.insert("Target", target);

    QJsonObject db =  selectionConfig["Database"].toObject();
    db.insert("File", "C:/SourceTree/Simcenter-EQSS/SelectGM/examples/NGAWest2-1000.csv");
    selectionConfig.insert("Database", db);

    QJsonDocument selectionConfigDoc(selectionConfig);
    QFile selectionConfigFile(selectionConfigPath);
    if (!selectionConfigFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't write record selection config file.");
        return;
    }
    selectionConfigFile.write(selectionConfigDoc.toJson());
    selectionConfigFile.close();

    QStringList args;
    args << selectionConfigPath << selectionOutputPath;
    m_recordSelectionProcess.start(selectRecordPath, args);

    if(!m_recordSelectionProcess.waitForStarted(-1))
    {
        emit statusUpdated("Failed to start ground motion record selection!");
        return;
    }
}

void ScenarioProcessor::processHazardOutput()
{
    QByteArray output = m_hazardAnalysisProcess.readAllStandardOutput();
    qDebug().nospace().noquote() << output;
    m_output = m_output.append(QString(output));
    processOutputLines();
}

void ScenarioProcessor::processSimulationOutput()
{
    QByteArray output = m_simulationProcess.readAllStandardOutput();
    qDebug().nospace().noquote() << output;
    m_output = m_output.append(QString(output));
    processOutputLines();
}

void ScenarioProcessor::processSelectionOutput()
{
    QByteArray output = m_recordSelectionProcess.readAllStandardOutput();
    qDebug().nospace().noquote() << output;
    m_output = m_output.append(QString(output));
    processOutputLines();
}

QString ScenarioProcessor::getWorkFilePath(QString filename)
{
    return QDir::cleanPath(m_workDir + QDir::separator() + filename);
}

void ScenarioProcessor::startProcessingOutputs()
{
    emit statusUpdated("Processing outputs...");

    //We will open the record selection output
    QString selectionOutputPath = getWorkFilePath("SelectionOutput.json");
    QFile selectionOutputFile(selectionOutputPath);
    if(!selectionOutputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Couldn't read record selection outputs!");
        return;
    }

    QString output = selectionOutputFile.readAll();
    QJsonDocument selectionOutputDoc = QJsonDocument::fromJson(output.toUtf8());
    QJsonArray selectionResults = selectionOutputDoc.object()["GroundMotions"].toArray();

    //We will also open the hazard analysis output to read the site location
    QString hazardOutputPath = getWorkFilePath("Scenario_" + m_intensityMeasure.type()+ ".json");
    QFile hazardOutputFile(hazardOutputPath);
    if(!hazardOutputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Couldn't read hazard analysis outputs!");
        return;
    }
    output = hazardOutputFile.readAll();
    QJsonDocument hazardOutputDoc = QJsonDocument::fromJson(output.toUtf8());
    QJsonArray hazardResults = hazardOutputDoc.object()["GroundMotions"].toArray();

    //Clearing results before adding new ones
    m_resultsModel.clear();
    for(int i = 0; i < selectionResults.size(); i++)
    {
        SiteResult* newResult = m_resultsModel.createNewResult();

        QJsonObject location = hazardResults[i].toObject()["Location"].toObject();
        newResult->location().set(location["Latitude"].toDouble(), location["Longitude"].toDouble());

        QJsonObject result = selectionResults[i].toObject();
        newResult->setRecordId(result["Record"].toObject()["Id"].toInt());
        newResult->setScaleFactor(result["ScaleFactor"].toDouble());

        QJsonArray imMeans;
        if(m_intensityMeasure.type() == "SA")
            imMeans = hazardResults[i].toObject()["SA"].toObject()["Mean"].toArray();
        else
            imMeans = hazardResults[i].toObject()["PGA"].toObject()["Mean"].toArray();
        QVector<double> means;
        QJsonValue mean;
        foreach (mean, imMeans)
            means.append(exp(mean.toDouble()));
        newResult->setMeans(means);
    }
    QJsonObject result = selectionResults[0].toObject();

    m_siteResult.setRecordId(result["Record"].toObject()["Id"].toInt());
    m_siteResult.setScaleFactor(result["ScaleFactor"].toDouble());
    emit progressUpdated("Finished record selection and scaling");
    emit statusUpdated("Analysis completed successfully!");
    emit finished();
    emit progressUpdated("");
}

void ScenarioProcessor::setupConnections()
{
    //connecting hazard analysis output to process hazard output slot
    connect(&m_hazardAnalysisProcess, &QProcess::readyReadStandardOutput, this, &ScenarioProcessor::processHazardOutput);

    //connecting simulation output to process simulation output slot
    connect(&m_simulationProcess, &QProcess::readyReadStandardOutput, this, &ScenarioProcessor::processSimulationOutput);

    //connecting selection output to process selection output slot
    connect(&m_recordSelectionProcess, &QProcess::readyReadStandardOutput, this, &ScenarioProcessor::processSelectionOutput);

    //connecting hazard analysis finish to simulation start
    connect(&m_hazardAnalysisProcess, QOverload<int>::of(&QProcess::finished), [this](int){this->startSimulation();});

    //connecting simulation finish to selection start
    connect(&m_simulationProcess, QOverload<int>::of(&QProcess::finished), [this](int){this->startSelection();});

    //connecting selection finish to process the outputs
    connect(&m_recordSelectionProcess, QOverload<int>::of(&QProcess::finished), [this](int){this->startProcessingOutputs();});
}

void ScenarioProcessor::processOutputLines()
{
    QString line;
    QStringList lines =  m_output.split(QRegExp("\n|\r\n|\r"), QString::SkipEmptyParts);
    m_output.clear();

    if(lines.count() > 100)
        for (int i = 0; i < lines.count() - 100; i++ )
            lines.removeAt(i);

    QRegularExpression siteExp("Processing Site ([0-9]+)");
    QRegularExpression selectionExp("Selecting record for target ([0-9]+)");

    foreach (line, lines)
    {
        emit progressUpdated(line.trimmed());

        QRegularExpressionMatch siteMatch = siteExp.match(line);
        if (siteMatch.hasMatch())
        {
            int siteNo = siteMatch.captured(1).toInt();
            emit progressUpdated((double)siteNo/m_siteConfig.siteGrid().getNumSites());
        }

        QRegularExpressionMatch selectionMatch = selectionExp.match(line);
        if (selectionMatch.hasMatch())
        {
            int targetNo = selectionMatch.captured(1).toInt();
            emit progressUpdated((double)targetNo/m_siteConfig.siteGrid().getNumSites());
        }
    }
}
