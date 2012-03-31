/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * Copyright (C) 2011 - 2012 MangosR2 <http://github.com/mangosR2/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_AHNKAHET_H
#define DEF_AHNKAHET_H
/* Encounters
 * Elder Nadox         = 1
 * Prince Taldram      = 2
 * Jedoga Shadowseeker = 3
 * Herald Volazj       = 4
 * Amanitar            = 5
*/

enum AhnkahetInstance
{
    MAX_ENCOUNTER           = 5,

    TYPE_NADOX              = 0,
    TYPE_TALDARAM           = 1,
    TYPE_JEDOGA             = 2,
    TYPE_VOLAZJ             = 3,
    TYPE_AMANITAR           = 4,

    GO_DOOR_TALDARAM        = 192236,
    GO_ANCIENT_DEVICE_L     = 193093,
    GO_ANCIENT_DEVICE_R     = 193094,
    GO_VORTEX               = 193564,

    NPC_AHNKAHAR_GUARDIAN         = 30176,
    NPC_AHNKAHAR_SWARMER          = 30178,

    NPC_TALDARAM            = 29308,
    NPC_JEDOGA_SHADOWSEEKER = 29310,
    NPC_TWILIGHT_INITIATE   = 30114,



    NPC_ELDER_NADOX             = 29309,
    NPC_AHNKAHAR_GUARDIAN_EGG   = 30173,
    NPC_AHNKAHAR_SWARM_EGG      = 30172,
    //NPC_JEDOGA_SHADOWSEEKER   = 29310,

    ACHIEV_START_VOLAZJ_ID      = 20382,

    ACHIEV_CRITERIA_VOLUNTEER_WORK      = 7359,
    ACHIEV_CRIT_RESPECT_ELDERS  = 7317,             // Nadox, achiev 2038
};

class MANGOS_DLL_DECL instance_ahnkahet : public ScriptedInstance
{
    public:
        instance_ahnkahet(Map* pMap);
        ~instance_ahnkahet() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);

        uint32 GetData(uint32 uiType);

        ObjectGuid SelectRandomGuardianEggGuid();
        ObjectGuid SelectRandomSwarmerEggGuid();

        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        bool m_bRespectElders;

        uint8 m_uiDevicesActivated;

        GUIDList m_GuardianEggList;
        GUIDList m_SwarmerEggList;
};

#endif
