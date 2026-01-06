class CfgPatches {
  class CustomEscMenu {
    units[] = {};
    weapons[] = {};
    requiredVersion = 0.1;
    requiredAddons[] = {"DZ_Data",
                        "DZ_Scripts",
                        "DZ_Gear_Consumables",
                        "DZ_Vehicles_Wheeled",
                        "DZ_Structures_Residential",
                        "DZ_Sounds_Effects",
                        "DZ_Sounds_Weapons"};
  };
};

class CfgMods {
  class CustomEscMenu {
    dir = "CustomEscMenu";
    picture = "";
    action = "";
    hideName = 1;
    hidePicture = 1;
    name = "Custom Escape Menu";
    credits = "SOSH79";
    author = "SOSH79";
    authorID = "0";
    version = "1.0";
    extra = 0;
    type = "mod";
    inputs = "CustomEscMenu/data/inputs.xml";
    dependencies[] = {"Game", "Mission"};
    class defs {
      class gameScriptModule {
        value = "";
        files[] = {"CustomEscMenu/Scripts/3_Game"};
      };
      class missionScriptModule {
        value = "";
        files[] = {"CustomEscMenu/Scripts/5_Mission"};
      };
    };
  };
};
