/* Copyright (C) 2010 -2012 by /dev/rsa for ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ICECROWN_SPIRE_H
#define DEF_ICECROWN_SPIRE_H
#include "BSW_instance.h"
#include "BSW_ai.h"

enum
{
    TYPE_TELEPORT               = 0,
    TYPE_MARROWGAR              = 1,
    TYPE_DEATHWHISPER           = 2,
    TYPE_FLIGHT_WAR             = 3,
    TYPE_SAURFANG               = 4,
    TYPE_FESTERGUT              = 5,
    TYPE_ROTFACE                = 6,
    TYPE_PUTRICIDE              = 7,
    TYPE_BLOOD_COUNCIL          = 8,
    TYPE_LANATHEL               = 9,
    TYPE_VALITHRIA              = 10,
    TYPE_SINDRAGOSA             = 11,
    TYPE_LICH_KING              = 13,
    MAX_ENCOUNTERS,

    TYPE_FROSTMOURNE_ROOM,
    TYPE_KINGS_OF_ICC,
    TYPE_ICECROWN_QUESTS,
    TYPE_STINKY,
    TYPE_PRECIOUS,

    NPC_LORD_MARROWGAR          = 36612,
    NPC_LADY_DEATHWHISPER       = 36855,
    NPC_DEATHBRINGER_SAURFANG   = 37813,
    NPC_FESTERGUT               = 36626,
    NPC_ROTFACE                 = 36627,
    NPC_PROFESSOR_PUTRICIDE     = 36678,
    NPC_TALDARAM                = 37973,
    NPC_VALANAR                 = 37970,
    NPC_KELESETH                = 37972,
    NPC_LANATHEL                = 37955,
    NPC_VALITHRIA               = 36789,
    NPC_VALITHRIA_QUEST         = 38589,
    NPC_SINDRAGOSA              = 36853,
    NPC_LICH_KING               = 36597,

    NPC_LANATHEL_INTRO          = 38004,
    NPC_BLOOD_ORB_CONTROL       = 38008,

    NPC_MURADIN                 = 36948,

    NPC_TIRION                  = 38995,
    NPC_MENETHIL                = 38579,

    NPC_FROSTMOURNE_TRIGGER     = 38584,
    NPC_FROSTMOURNE_HOLDER      = 27880,

    NPC_STINKY                  = 37025,
    NPC_PRECIOUS                = 37217,

    NPC_RIMEFANG                = 37533,
    NPC_SPINESTALKER            = 37534,

    NPC_COMBAT_TRIGGER          = 38752,

    GO_TELEPORT_GOSSIP_MESSAGE  = 99323,
    TELEPORT_GOSSIP_MESSAGE     = 99322,

    GO_ICEWALL_1                = 201911,
    GO_ICEWALL_2                = 201910,

    GO_MARROWGAR_DOOR           = 201857,

    GO_ORATORY_DOOR             = 201563,
    GO_DEATHWHISPER_ELEVATOR    = 202220, //5653

    GO_SAURFANG_DOOR            = 201825,

    GO_GAS_RELEASE_VALVE        = 201616, //72479

    GO_ORANGE_PLAGUE            = 201371, //72536
    GO_GREEN_PLAGUE             = 201370, //72537

    GO_SCIENTIST_DOOR_GREEN     = 201614, //72530
    GO_SCIENTIST_DOOR_ORANGE    = 201613, //72531
    GO_SCIENTIST_DOOR_COLLISION = 201612,
    GO_SCIENTIST_DOOR           = 201372, //72541

    GO_BLOODWING_DOOR           = 201920, //72532
    GO_CRIMSON_HALL_DOOR        = 201376, //72532
    GO_COUNCIL_DOOR_1           = 201377, //72533
    GO_COUNCIL_DOOR_2           = 201378, //72534

    GO_BLOODPRINCE_DOOR         = 201746,
    GO_ICECROWN_GRATE           = 201755,

    GO_FROSTWING_DOOR           = 201919,
    GO_GREEN_DRAGON_DOOR_1      = 201375, //1202
    GO_GREEN_DRAGON_DOOR_2      = 201374, //1200
    GO_VALITHRIA_DOOR_1         = 201381, //1618
    GO_VALITHRIA_DOOR_2         = 201382, //1482
    GO_VALITHRIA_DOOR_3         = 201383, //1335
    GO_VALITHRIA_DOOR_4         = 201380, //1558

    GO_SINDRAGOSA_DOOR_1        = 201369, //1619
    GO_SINDRAGOSA_DOOR_2        = 201379,

    GO_SINDRAGOSA_ENTRANCE      = 201373,

    GO_FROZENTRONE_TR           = 202223, //72061

    GO_SAURFANG_CACHE_10        = 202239,
    GO_SAURFANG_CACHE_25        = 202240,
    GO_SAURFANG_CACHE_10_H      = 202238,
    GO_SAURFANG_CACHE_25_H      = 202241,

    GO_GUNSHIP_ARMORY_A_10      = 201872, //
    GO_GUNSHIP_ARMORY_A_25      = 201873, //
    GO_GUNSHIP_ARMORY_A_10H     = 201874, //
    GO_GUNSHIP_ARMORY_A_25H     = 201875, //

    GO_GUNSHIP_ARMORY_H_10      = 202177, //
    GO_GUNSHIP_ARMORY_H_25      = 202178, //
    GO_GUNSHIP_ARMORY_H_10H     = 202179, //
    GO_GUNSHIP_ARMORY_H_25H     = 202180, //

    GO_DREAMWALKER_CACHE_10      = 201959, //
    GO_DREAMWALKER_CACHE_25      = 202339, //
    GO_DREAMWALKER_CACHE_10_H    = 202338, //
    GO_DREAMWALKER_CACHE_25_H    = 202340, //

    GO_PLAGUE_SIGIL             = 202182,
    GO_FROSTWING_SIGIL          = 202181,
    GO_BLOODWING_SIGIL          = 202183,

    GO_ICESHARD_1               = 202142, //8304
    GO_ICESHARD_2               = 202141, //8364
    GO_ICESHARD_3               = 202143, //8310
    GO_ICESHARD_4               = 202144, //9007

    GO_FROSTY_WIND              = 202188, //
    GO_FROSTY_EDGE              = 202189, //
    GO_SNOW_EDGE                = 202190, //
    GO_ARTHAS_PLATFORM          = 202161, //
    GO_ARTHAS_PRECIPICE         = 202078, //

    TYPE_EVENT_TIMER            = 99,
    TYPE_EVENT                  = 100,
    TYPE_EVENT_NPC              = 101,
    MAP_NUM                     = 631,
    DATA_DIRECTION              = 1001,
    DATA_BLOOD_INVOCATION       = 1002,
    DESPAWN_TIME                = 300000,
    SPELL_SHADOWS_EDGE          = 71168,

};

class MANGOS_DLL_DECL instance_icecrown_spire : public ScriptedInstance
{
public:
    instance_icecrown_spire(Map* pMap);
    ~instance_icecrown_spire() {}

    void Initialize();

    void OnObjectCreate(GameObject* pGo);
    void OnCreatureCreate(Creature* pCreature);

    bool IsEncounterInProgress();

    void SetData(uint32 uiType, uint32 uiData);
    uint32 GetData(uint32 uiType);

    const char* Save() { return strSaveData.c_str(); }
    void Load(const char* chrIn);

private:

    uint8 Difficulty;
    bool needSave;
    std::string strSaveData;

    //Creatures GUID
    uint32 m_auiEncounter[MAX_ENCOUNTERS+1];

    uint32 m_uiCouncilInvocation;

    uint32 m_auiEvent;
    uint32 m_auiEventTimer;
    uint32 m_uiDirection;

    uint32 m_uiStinkystate;
    uint32 m_uiPreciousstate;
    uint32 m_uiGunshipArmoryA_ID;
    uint32 m_uiGunshipArmoryH_ID;
    uint32 m_uiValithriaCache;
    uint32 m_uiSaurfangCache;
};

struct MANGOS_DLL_DECL base_icc_bossAI : public ScriptedAI
{
    base_icc_bossAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_uiMapDifficulty = pCreature->GetMap()->GetDifficulty();
        m_bIsHeroic = m_uiMapDifficulty > RAID_DIFFICULTY_25MAN_NORMAL;
        m_bIs25Man = (m_uiMapDifficulty == RAID_DIFFICULTY_25MAN_NORMAL || m_uiMapDifficulty == RAID_DIFFICULTY_25MAN_HEROIC);
        Reset();
    }

    ScriptedInstance* m_pInstance;
    Difficulty m_uiMapDifficulty;
    bool m_bIsHeroic;
    bool m_bIs25Man;

    void Reset(){}
    void UpdateAI(const uint32 uiDiff){}

    Unit* SelectRandomRangedTarget(Unit *pSource)
    {
        Unit *pResult = NULL;
        std::list<Unit*> lTargets;
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();

        for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
        {
            if (!(*i)->getUnitGuid().IsPlayer())
                continue;

            if (Unit* pTmp = m_creature->GetMap()->GetUnit((*i)->getUnitGuid()))
                lTargets.push_back(pTmp);
        }

        if (!lTargets.empty())
        {
            uint8 max = m_bIs25Man ? 8 : 3;
            std::list<Unit*>::iterator iter;

            lTargets.sort(ObjectDistanceOrderReversed(pSource));
            iter = lTargets.begin();

            if (max >= lTargets.size())
                max = lTargets.size() - 1;

            std::advance(iter, urand(0, max));
            pResult = (*iter);
        }

        return pResult;
    }
};

enum AchievementCriteriaIds
{
    // Lord Marrowgar
    CRITERIA_BONED_10N                  = 12775,
    CRITERIA_BONED_25N                  = 12962,
    CRITERIA_BONED_10H                  = 13393,
    CRITERIA_BONED_25H                  = 13394,

    // Rotface
    CRITERIA_DANCES_WITH_OOZES_10N      = 12984,
    CRITERIA_DANCES_WITH_OOZES_25N      = 12966,
    CRITERIA_DANCES_WITH_OOZES_10H      = 12985,
    CRITERIA_DANCES_WITH_OOZES_25H      = 12983,

    // Professor Putricide
    CRITERIA_NAUSEA_10N                 = 12987,
    CRITERIA_NAUSEA_25N                 = 12968,
    CRITERIA_NAUSEA_10H                 = 12988,
    CRITERIA_NAUSEA_25H                 = 12981,

    // Blood Prince Council
    CRITERIA_ORB_WHISPERER_10N          = 13033,
    CRITERIA_ORB_WHISPERER_25N          = 12969,
    CRITERIA_ORB_WHISPERER_10H          = 13034,
    CRITERIA_ORB_WHISPERER_25H          = 13032,

    // Blood-Queen Lana'thel
    CRITERIA_KILL_LANA_THEL_10M         = 13340,
    CRITERIA_KILL_LANA_THEL_25M         = 13360,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_10N  = 12780,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_25N  = 13012,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_10V  = 13011,
    CRITERIA_ONCE_BITTEN_TWICE_SHY_25V  = 13013,
};

#endif
