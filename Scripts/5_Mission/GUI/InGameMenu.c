modded class InGameMenu
{
    protected Widget m_DiscordButton;
    protected Widget m_DonationButton;
    protected Widget m_InfoButton;
    
    protected TextWidget m_PlayerNameTag;
    protected TextWidget m_StatZombies;
    protected TextWidget m_StatPlayers;
    protected TextWidget m_StatDistance;
    protected TextWidget m_StatLongestShot;

    void ~InGameMenu()
    {
        GetRPCManager().RemoveRPC("CustomEscMenuRPC", "GetConfigResponse");
    }

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("CustomEscMenu/GUI/layouts/custom_ingamemenu.layout");
        
        // Register RPC for config
        GetRPCManager().AddRPC("CustomEscMenuRPC", "GetConfigResponse", this, SingeplayerExecutionType.Client);
        
        m_ContinueButton			= layoutRoot.FindAnyWidget("continuebtn");
        m_ExitButton				= layoutRoot.FindAnyWidget("exitbtn");
        m_OptionsButton				= layoutRoot.FindAnyWidget("optionsbtn");
        
        m_DiscordButton             = layoutRoot.FindAnyWidget("discordbtn");
        m_DonationButton            = layoutRoot.FindAnyWidget("donationbtn");
        m_InfoButton                = layoutRoot.FindAnyWidget("infobtn");
        
        m_PlayerNameTag             = TextWidget.Cast(layoutRoot.FindAnyWidget("player_name_tag"));
        m_StatZombies               = TextWidget.Cast(layoutRoot.FindAnyWidget("stat_zombies"));
        m_StatPlayers               = TextWidget.Cast(layoutRoot.FindAnyWidget("stat_players"));
        m_StatDistance              = TextWidget.Cast(layoutRoot.FindAnyWidget("stat_distance"));
        m_StatLongestShot           = TextWidget.Cast(layoutRoot.FindAnyWidget("stat_longest_shot"));
        
        // Hide vanilla elements that we don't use in this specific design
        m_RestartButton				= layoutRoot.FindAnyWidget("restartbtn");
        m_RespawnButton 			= layoutRoot.FindAnyWidget("respawn_button");
        m_RestartDeadRandomButton	= layoutRoot.FindAnyWidget("respawn_button_random");
        m_RestartDeadCustomButton	= layoutRoot.FindAnyWidget("respawn_button_custom");
        m_ModdedWarning				= TextWidget.Cast(layoutRoot.FindAnyWidget("ModdedWarning"));
        m_ServerInfoPanel 			= layoutRoot.FindAnyWidget("server_info");
        
        UpdatePlayerStats();

        // High-Force Whitelist Cleanup (Nuclear Option)
        NuclearCleanup();

        HudShow(false);
        
        Mission mission = g_Game.GetMission();
        if (mission)
            mission.Pause();
        
        return layoutRoot;
    }

    override void OnShow()
    {
        super.OnShow();
        NuclearCleanup();
        
        // Request config for links
        GetRPCManager().SendRPC("CustomEscMenuRPC", "GetConfigRequest", null, true, null);
    }

    override void UpdateGUI()
    {
        super.UpdateGUI();
        // Check for any unauthorized widgets every frame (minimal overhead)
        NuclearCleanup();
    }

    protected void NuclearCleanup()
    {
        if (!layoutRoot) return;

        Widget child = layoutRoot.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            if (!IsProtectedWidget(child.GetName()))
            {
                child.Show(false);
                child.Unlink();
            }
            child = next;
        }
    }

    protected bool IsProtectedWidget(string name)
    {
        switch (name)
        {
            case "background_image":
            case "top_left_panel":
            case "server_logo":
            case "top_right_panel":
            case "session_title":
            case "player_name_tag":
            case "stat_zombies":
            case "stat_players":
            case "stat_distance":
            case "stat_longest_shot":
            case "bottom_left_panel":
            case "discordbtn":
            case "discordbtn_label":
            case "donationbtn":
            case "donationbtn_label":
            case "bottom_right_panel":
            case "continuebtn":
            case "continuebtn_label":
            case "infobtn":
            case "infobtn_label":
            case "optionsbtn":
            case "optionsbtn_label":
            case "exitbtn":
            case "exitbtn_label":
            case "respawn_button":
            case "respawn_button_label":
            case "respawn_button_random":
            case "respawn_button_random_label":
            case "respawn_button_custom":
            case "respawn_button_custom_label":
            case "vanilla_compat_hidden":
                return true;
        }
        return false;
    }

    void UpdatePlayerStats()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        if (m_PlayerNameTag)
            m_PlayerNameTag.SetText("NAME: " + player.GetIdentity().GetName());

        if (m_StatZombies)
        {
            int zombies = player.StatGet(AnalyticsManagerServer.STAT_INFECTED_KILLED);
            if (zombies < 0) zombies = 0;
            m_StatZombies.SetText("ZOMBIES KILLED: " + zombies.ToString());
        }

        if (m_StatPlayers)
        {
            int players = player.StatGet(AnalyticsManagerServer.STAT_PLAYERS_KILLED);
            if (players < 0) players = 0;
            m_StatPlayers.SetText("PLAYERS KILLED: " + players.ToString());
        }

        if (m_StatDistance)
        {
            float dist = player.StatGet(AnalyticsManagerServer.STAT_DISTANCE);
            if (dist < 0) dist = 0;
            m_StatDistance.SetText("DISTANCE WALKED: " + GetDistanceString(dist));
        }

        if (m_StatLongestShot)
        {
            float longest = player.StatGet(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT);
            if (longest < 0) longest = 0;
            m_StatLongestShot.SetText("LONGEST SHOT: " + GetDistanceString(longest));
        }
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

    // Config RPC Handler
    void GetConfigResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client || !GetGame().IsClient()) return;

        Param1<CustomEscMenuConfig> data;
        if (!ctx.Read(data)) return;

        CustomEscMenuConfig config = data.param1;
        CustomEscMenuConfigManager.SetConfig(config);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_DiscordButton)
        {
          string discord = "https://discord.gg/nWbaERkAQR";
          CustomEscMenuConfig config = CustomEscMenuConfigManager.GetConfig();
          if (config && config.DiscordLink != "") discord = config.DiscordLink;
          
          GetGame().OpenURL(discord);
          return true;
        }
        else if (w == m_DonationButton)
        {
          string donation = "https://www.gtxgaming.co.uk/clientarea/index.php?m=public_pay&hash_id=WY0o0rNgGQzv";
          CustomEscMenuConfig config2 = CustomEscMenuConfigManager.GetConfig();
          if (config2 && config2.DonationLink != "") donation = config2.DonationLink;
          
          GetGame().OpenURL(donation);
          return true;
        }
        else if (w == m_InfoButton)
        {
            GetGame().GetUIManager().EnterScriptedMenu(CUSTOM_INTEL_MENU, this);
            return true;
        }

        return super.OnClick(w, x, y, button);
    }
}
