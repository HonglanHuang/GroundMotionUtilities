/**
 * 
 */
package org.designsafe.ci.simcenter;


import com.google.gson.annotations.SerializedName;

/**
 * @author Wael Elhaddad
 *
 */
public class EQScenarioConfig {
	
	@SerializedName("Site")
	private SiteConfig m_Site;
	
	@SerializedName("EqRupture")
	private EqRuptureConfig m_EqRupture;
	
	@SerializedName("GMPE")
	private GMPEConfig m_GMPE;
	
	@SerializedName("IntensityMeasure")
	private IMConfig m_IntensityMeasure;
	
	public EQScenarioConfig(SiteConfig site, EqRuptureConfig rupture, GMPEConfig gmpe, IMConfig im)
	{
		this.m_Site = site;
		this.m_EqRupture = rupture;
		this.m_GMPE = gmpe;
		this.m_IntensityMeasure = im;
	}
	
	public SiteConfig GetSiteConfig()
	{
		return this.m_Site;
	}
	
	public EqRuptureConfig GetRuptureConfig()
	{
		return this.m_EqRupture;
	}
	
	public GMPEConfig GetGMPEConfig()
	{
		return this.m_GMPE;
	}
	
	public IMConfig GetIMConfig()
	{
		return this.m_IntensityMeasure;
	}
}
