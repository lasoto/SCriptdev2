/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_UTG_KEEP_H
#define DEF_UTG_KEEP_H

enum
{
    MAX_ENCOUNTER               = 6,

    TYPE_KELESETH               = 0,
    TYPE_SKARVALD_DALRONN       = 1,
    TYPE_INGVAR                 = 2,
    TYPE_BELLOW_1               = 3,
    TYPE_BELLOW_2               = 4,
    TYPE_BELLOW_3               = 5,

    NPC_KELESETH                = 23953,
    NPC_SKARVALD                = 24200,
    NPC_DALRONN                 = 24201,
    NPC_INGVAR                  = 23954,

    NPC_FROST_TOMB              = 23965,

    GO_BELLOW_1                 = 186688,
    GO_BELLOW_2                 = 186689,
    GO_BELLOW_3                 = 186690,
    GO_FORGEFIRE_1              = 186692,
    GO_FORGEFIRE_2              = 186693,
    GO_FORGEFIRE_3              = 186691,

    GO_EXITDOOR_1               = 186756,
    GO_EXITDOOR_2               = 186694,

    ACHIEV_CRIT_ON_THE_ROCKS    = 7231,
};

class MANGOS_DLL_DECL instance_utgarde_keep : public ScriptedInstance
{
    public:
        instance_utgarde_keep(Map* pMap);
        ~instance_utgarde_keep() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void OnCreatureDeath(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        bool m_bKelesethAchievFailed;
};

#endif
