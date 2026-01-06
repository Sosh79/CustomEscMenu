class CustomPlayerInfoDisplay
{
    protected Widget m_RootWidget;
    protected PlayerBase m_Player;
    
    protected ProgressBarWidget m_HealthBar;
    protected ProgressBarWidget m_BloodBar;
    protected ProgressBarWidget m_WaterBar;
    protected ProgressBarWidget m_EnergyBar;
    
    protected TextWidget m_SurvivalTime;
    protected TextWidget m_Distance;
    protected TextWidget m_ZKilled;
    protected TextWidget m_PKilled;
    protected TextWidget m_LongestShot;
    
    void CustomPlayerInfoDisplay()
    {
    }
    
    void Init(Widget root)
    {
        m_RootWidget = GetGame().GetWorkspace().CreateWidgets("CustomEscMenu/GUI/layouts/custom_playerinfo.layout", root);
        
        m_HealthBar = ProgressBarWidget.Cast(m_RootWidget.FindAnyWidget("HealthBar"));
        m_BloodBar = ProgressBarWidget.Cast(m_RootWidget.FindAnyWidget("BloodBar"));
        m_WaterBar = ProgressBarWidget.Cast(m_RootWidget.FindAnyWidget("WaterBar"));
        m_EnergyBar = ProgressBarWidget.Cast(m_RootWidget.FindAnyWidget("EnergyBar"));
        
        m_SurvivalTime = TextWidget.Cast(m_RootWidget.FindAnyWidget("txt_survivalTime"));
        m_Distance = TextWidget.Cast(m_RootWidget.FindAnyWidget("txt_distance"));
        m_ZKilled = TextWidget.Cast(m_RootWidget.FindAnyWidget("txt_zKilled"));
        m_PKilled = TextWidget.Cast(m_RootWidget.FindAnyWidget("txt_pKilled"));
        m_LongestShot = TextWidget.Cast(m_RootWidget.FindAnyWidget("txt_longestShot"));
        
        // Register RPC
        GetRPCManager().AddRPC("CustomEscMenuRPC", "SyncPlayerStats", this, SingeplayerExecutionType.Client);
        
        RequestUpdate();
    }
    
    void RequestUpdate()
    {
        if (GetGame().IsClient())
        {
            GetRPCManager().SendRPC("CustomEscMenuRPC", "SyncPlayerStatsRequest", null, true, null);
        }
    }
    
    // RPC Handler
    void SyncPlayerStats(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client || !GetGame().IsClient()) return;

        Param2<array<int>, array<float>> data;
        if (!ctx.Read(data)) return;

        array<int> plyIData = data.param1;
        array<float> plyFData = data.param2;

        if (plyIData.Count() >= 4)
        {
            m_HealthBar.SetCurrent(plyIData[0]);
            m_BloodBar.SetCurrent(plyIData[1]);
            m_WaterBar.SetCurrent(plyIData[2]);
            m_EnergyBar.SetCurrent(plyIData[3]);
        }

        if (plyFData.Count() >= 5)
        {
            m_Distance.SetText(FormatDistance(plyFData[0]));
            m_SurvivalTime.SetText(FormatTime(plyFData[1]));
            m_PKilled.SetText(Math.Round(plyFData[2]).ToString());
            m_ZKilled.SetText(Math.Round(plyFData[3]).ToString());
            m_LongestShot.SetText(FormatDistance(plyFData[4]));
        }
    }
    
    void UpdateStatus()
    {
        // This can still do local basic updates or just trigger an RPC request
        RequestUpdate();
    }
    
    protected string FormatTime(float seconds)
    {
        int totalSecs = (int)seconds;
        int h = totalSecs / 3600;
        int m = (totalSecs % 3600) / 60;
        return h.ToStringLen(2) + ":" + m.ToStringLen(2);
    }
    
    protected string FormatDistance(float meters)
    {
        if (meters >= 1000)
        {
            float km = meters / 1000.0;
            return km.ToString() + " km";
        }
        return Math.Round(meters).ToString() + " m";
    }
}
