class CustomIntelPanel extends UIScriptedMenu
{
    protected Widget m_RulesTab;
    protected Widget m_StatusTab;
    protected Widget m_ServerTab;
    
    protected Widget m_RulesContent;
    protected Widget m_StatusContent;
    protected Widget m_ServerContent;
    
    protected RichTextWidget m_RulesText;
    
    protected Widget m_CloseBtn;
    
    protected ref CustomPlayerInfoDisplay m_PlayerInfoDisplay;
    
    protected TextWidget m_TitleText;
    
    protected TextWidget m_UptimeVal;
    protected TextWidget m_PlayerCountVal;
    protected MultilineTextWidget m_PlayerListText;
    
    void ~CustomIntelPanel()
    {
        GetRPCManager().RemoveRPC("CustomEscMenuRPC", "SyncPlayerCount");
        GetRPCManager().RemoveRPC("CustomEscMenuRPC", "GetConfigResponse");
        GetRPCManager().RemoveRPC("CustomEscMenuRPC", "SyncPlayerList");
    }

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("CustomEscMenu/GUI/layouts/infopanel.layout");
        
        m_RulesTab = layoutRoot.FindAnyWidget("tab_rules");
        m_StatusTab = layoutRoot.FindAnyWidget("tab_status");
        m_ServerTab = layoutRoot.FindAnyWidget("tab_server");
        
        m_RulesContent = layoutRoot.FindAnyWidget("rules_content");
        m_StatusContent = layoutRoot.FindAnyWidget("status_content");
        m_ServerContent = layoutRoot.FindAnyWidget("server_content");
        
        m_RulesText = RichTextWidget.Cast(layoutRoot.FindAnyWidget("rules_text"));
        
        m_CloseBtn = layoutRoot.FindAnyWidget("close_btn");
        
        m_PlayerInfoDisplay = new CustomPlayerInfoDisplay();
        m_PlayerInfoDisplay.Init(layoutRoot.FindAnyWidget("status_content"));
        
        m_TitleText = TextWidget.Cast(layoutRoot.FindAnyWidget("title_text"));
        
        m_UptimeVal = TextWidget.Cast(layoutRoot.FindAnyWidget("server_uptime"));
        m_PlayerCountVal = TextWidget.Cast(layoutRoot.FindAnyWidget("player_count"));
        m_PlayerListText = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("player_list_text"));
        
        // Register RPCs
        GetRPCManager().AddRPC("CustomEscMenuRPC", "SyncPlayerCount", this, SingeplayerExecutionType.Client);
        GetRPCManager().AddRPC("CustomEscMenuRPC", "GetConfigResponse", this, SingeplayerExecutionType.Client);
        GetRPCManager().AddRPC("CustomEscMenuRPC", "SyncPlayerList", this, SingeplayerExecutionType.Client);
        
        UpdateStatus();
        
        return layoutRoot;
    }

    protected void UpdateStatus()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        m_PlayerInfoDisplay.UpdateStatus();
        
        // Server Info (Placeholder)
        m_PlayerCountVal.SetText("PLAYERS ONLINE: --");
    }

    override void Update(float timeslice)
    {
        super.Update(timeslice);
        
        if (GetGame().GetInput().LocalPress("UAUIBack", false))
        {
            Close();
        }
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_CloseBtn)
        {
            Close();
            return true;
        }
        else if (w == m_RulesTab)
        {
            ShowTab(0);
            return true;
        }
        else if (w == m_StatusTab)
        {
            ShowTab(1);
            m_PlayerInfoDisplay.UpdateStatus();
            return true;
        }
        else if (w == m_ServerTab)
        {
            ShowTab(2);
            UpdateServerTab();
            return true;
        }
        
        return super.OnClick(w, x, y, button);
    }

    protected void ShowTab(int index)
    {
        m_RulesContent.Show(index == 0);
        m_StatusContent.Show(index == 1);
        m_ServerContent.Show(index == 2);
    }

    override void OnShow()
    {
        super.OnShow();
        // Request fresh data from server when menu is shown
        UpdateServerTab();
        GetRPCManager().SendRPC("CustomEscMenuRPC", "GetConfigRequest", null, true, null);
        
        // Set local cached config if available
        CustomEscMenuConfig config = CustomEscMenuConfigManager.GetConfig();
        UpdateUIFromConfig(config);
    }

    protected void UpdateUIFromConfig(CustomEscMenuConfig config)
    {
        if (!config) return;

        if (m_TitleText && config.SERVERNAME != "")
        {
            m_TitleText.SetText(config.SERVERNAME);
        }

        if (m_RulesText && config.ServerRules)
        {
            string rulesString = "";
            foreach (string line : config.ServerRules)
            {
                rulesString += line;
            }
            m_RulesText.SetText(rulesString);
        }
        
        if (m_StatusTab)
            m_StatusTab.Show(config.DISPLAYPLAYERSTATUS);
            
        if (m_ServerTab)
            m_ServerTab.Show(config.DISPLAYSERVER);
    }

    // This function runs on the CLIENT
    void SyncPlayerCount(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client || !GetGame().IsClient()) return;

        Param1<int> data;
        if (!ctx.Read(data)) return;

        int count = data.param1;
        if (m_PlayerCountVal)
        {
            m_PlayerCountVal.SetText("PLAYERS ONLINE: " + count.ToString());
        }
    }

    void SyncPlayerList(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client || !GetGame().IsClient()) return;

        Param1<array<string>> data;
        if (!ctx.Read(data)) return;

        array<string> playerNames = data.param1;
        string list = "";
        for (int i = 0; i < playerNames.Count(); i++)
        {
            list += (i + 1).ToString() + ". " + playerNames[i] + "\n";
        }
        
        if (m_PlayerListText)
            m_PlayerListText.SetText(list);
            
        if (m_PlayerCountVal)
            m_PlayerCountVal.SetText("PLAYERS ONLINE: " + playerNames.Count().ToString());
    }
    
    protected void UpdateServerTab()
    {
        GetRPCManager().SendRPC("CustomEscMenuRPC", "SyncPlayerCountRequest", null, true, null);
        GetRPCManager().SendRPC("CustomEscMenuRPC", "SyncPlayerListRequest", null, true, null);
    }

    void GetConfigResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client || !GetGame().IsClient()) return;

        Param1<CustomEscMenuConfig> data;
        if (!ctx.Read(data)) return;

        CustomEscMenuConfig config = data.param1;
        CustomEscMenuConfigManager.SetConfig(config);

        UpdateUIFromConfig(config);
    }
}
