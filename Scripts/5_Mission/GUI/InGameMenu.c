modded class InGameMenu
{
    protected Widget m_DiscordButton;
    protected Widget m_DonationButton;
    
    protected TextWidget m_PlayerNameTag;
    protected TextWidget m_StatZombies;
    protected TextWidget m_StatPlayers;
    protected TextWidget m_StatDistance;
    protected TextWidget m_StatLongestShot;

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("CustomEscMenu/GUI/layouts/custom_ingamemenu.layout");
        
        m_ContinueButton			= layoutRoot.FindAnyWidget("continuebtn");
        m_ExitButton				= layoutRoot.FindAnyWidget("exitbtn");
        m_OptionsButton				= layoutRoot.FindAnyWidget("optionsbtn");
        
        m_DiscordButton             = layoutRoot.FindAnyWidget("discordbtn");
        m_DonationButton            = layoutRoot.FindAnyWidget("donationbtn");
        
        m_PlayerNameTag             = TextWidget.Cast(layoutRoot.FindAnyWidget("player_name_tag"));
        m_StatZombies               = TextWidget.Cast(layoutRoot.FindAnyWidget("stat_zombies"));
        m_StatPlayers               = TextWidget.Cast(layoutRoot.FindAnyWidget("stat_players"));
        m_StatDistance              = TextWidget.Cast(layoutRoot.FindAnyWidget("stat_distance"));
        m_StatLongestShot           = TextWidget.Cast(layoutRoot.FindAnyWidget("stat_longest_shot"));
        
        // Hide vanilla elements that we don't use in this specific design
        m_RestartButton				= layoutRoot.FindAnyWidget("restartbtn");
        m_RespawnButton 			= layoutRoot.FindAnyWidget("respawn_button");
        m_ModdedWarning				= TextWidget.Cast(layoutRoot.FindAnyWidget("ModdedWarning"));
        m_ServerInfoPanel 			= layoutRoot.FindAnyWidget("server_info");
        
        UpdatePlayerStats();

        HudShow(false);
        
        Mission mission = g_Game.GetMission();
        if (mission)
            mission.Pause();
        
        return layoutRoot;
    }

    void UpdatePlayerStats()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        if (m_PlayerNameTag)
            m_PlayerNameTag.SetText("NAME: " + player.GetIdentity().GetName());

        if (m_StatZombies)
            m_StatZombies.SetText("ZOMBIES KILLED: " + player.StatGet(AnalyticsManagerServer.STAT_INFECTED_KILLED).ToString());

        if (m_StatPlayers)
            m_StatPlayers.SetText("PLAYERS KILLED: " + player.StatGet(AnalyticsManagerServer.STAT_PLAYERS_KILLED).ToString());

        if (m_StatDistance)
            m_StatDistance.SetText("DISTANCE WALKED: " + GetDistanceString(player.StatGet(AnalyticsManagerServer.STAT_DISTANCE)));

        if (m_StatLongestShot)
            m_StatLongestShot.SetText("LONGEST SHOT: " + GetDistanceString(player.StatGet(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT)));
    }

    protected string GetDistanceString(float total_distance)
    {
        if (total_distance <= 0)
            return "0 m";

        if (total_distance >= 1000)
        {
            float km = total_distance / 1000.0;
            // Round to 1 decimal place and convert to string
            // We use a simple trick to cut off decimals: multiply by 10, cast to int, then back to float / 10
            int km_x10 = km * 10;
            float km_rounded = km_x10 / 10.0;
            return km_rounded.ToString() + " km";
        }

        // Cast to int for whole meters
        int meters = total_distance;
        return meters.ToString() + " m";
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_DiscordButton)
        {
          GetGame().OpenURL("https://discord.gg/nWbaERkAQR");
          return true;
        }
        else if (w == m_DonationButton)
        {
          GetGame().OpenURL("https://www.gtxgaming.co.uk/clientarea/index.php?m=public_pay&hash_id=WY0o0rNgGQzv");
          return true;
        }

        return super.OnClick(w, x, y, button);
    }
}
