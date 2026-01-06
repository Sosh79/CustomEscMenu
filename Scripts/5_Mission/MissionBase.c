modded class MissionBase
{
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        UIScriptedMenu menu = super.CreateScriptedMenu(id);
        if (!menu)
        {
            switch (id)
            {
                case CUSTOM_INTEL_MENU:
                    menu = new CustomIntelPanel();
                    break;
            }
            if (menu)
            {
                menu.SetID(id);
            }
        }
        return menu;
    }
}
