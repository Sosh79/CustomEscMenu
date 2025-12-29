modded class LogoutMenu
{
    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets("CustomEscMenu/GUI/layouts/custom_logout_dialog.layout");
        
        m_LogoutTimeText    = TextWidget.Cast(layoutRoot.FindAnyWidget("txtLogoutTime"));
        m_DescriptionText   = TextWidget.Cast(layoutRoot.FindAnyWidget("txtDescription"));
        m_bLogoutNow        = ButtonWidget.Cast(layoutRoot.FindAnyWidget("bLogoutNow"));
        m_bCancel           = ButtonWidget.Cast(layoutRoot.FindAnyWidget("bCancel"));
        
        #ifndef PLATFORM_CONSOLE
        if (layoutRoot.FindAnyWidget("toolbar_bg"))
            layoutRoot.FindAnyWidget("toolbar_bg").Show(false);
        #endif
        
        UpdateInfo();
        
        // player should sit down if possible
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player && player.GetEmoteManager() && !player.IsRestrained() && !player.IsUnconscious()) 
        {
            player.GetEmoteManager().SetClientLoggingOut(true);
        }

        return layoutRoot;
    }

    override void SetTime(int time)
    {
        m_iTime = time;
        string text = "LOGOUT IN ";
        
        TimeConversions.ConvertSecondsToFullTime(time, m_FullTime);
        
        if (m_FullTime.m_Days > 0)
            text += m_FullTime.m_Days.ToString() + "d " + m_FullTime.m_Hours.ToString() + "h " + m_FullTime.m_Minutes.ToString() + "m " + m_FullTime.m_Seconds.ToString() + "s";
        else if (m_FullTime.m_Hours > 0)
            text += m_FullTime.m_Hours.ToString() + "h " + m_FullTime.m_Minutes.ToString() + "m " + m_FullTime.m_Seconds.ToString() + "s";
        else if (m_FullTime.m_Minutes > 0)
            text += m_FullTime.m_Minutes.ToString() + "m " + m_FullTime.m_Seconds.ToString() + "s";
        else
            text += m_FullTime.m_Seconds.ToString() + "s";
            
        m_LogoutTimeText.SetText(text);
    }
}
