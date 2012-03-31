/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
ScriptData
SDName: boss_the_lich_king
SD%Complete: 99%
SDComment:  by michalpolko with special thanks to:
            mangosR2 team and all who are supporting us with feedback, testing and fixes
            TrinityCore for some info about spells IDs
            everybody whom I forgot to mention here ;)

SDCategory: Icecrown Citadel
*/

#include "precompiled.h"
#include "icecrown_citadel.h"

enum BossSpells
{
    SPELL_BERSERK               = 47008,
    SPELL_SIT_EMOTE_NO_SHEATH   = 73220,
    SPELL_PLAGUE_AVOIDANCE      = 72846,

    // Intro
    SPELL_ICE_LOCK              = 71614,

    // Outro
    SPELL_FURY_OF_FROSTMOURNE   = 72350,
    SPELL_FURY_OF_FROSTMOURNE2  = 72351, // cannot resurect aura
    SPELL_RAISE_DEAD            = 71769,
    SPELL_THROW_FROSTMOURNE     = 73017, // 1
    SPELL_BROKEN_FROSTMOURNE    = 72398, // 2
    SPELL_SUMMON_FROSTMOURNE    = 72407, // 3 summon npc which casts 4 and 5 and LK enters this npc as vehicle
    SPELL_FROSTMOURNE_DESPAWN   = 72726, // 4
    SPELL_FROSTMOURNE_SPIRITS   = 72405, // 5
    SPELL_SOUL_BARRAGE          = 72305, // strangulation and sounds
    SPELL_LK_CINEMATIC          = 73159,

    // Tirion
    SPELL_LIGHTS_BLESSING       = 71797, // after 5secs smashes Ice Lock

    // Terenas Menethil
    SPELL_MASS_RESURRECTION     = 72429, // dummy
    SPELL_MASS_RESURRECTION2    = 72423, // actual res

    // Phase One
    SPELL_NECROTIC_PLAGUE       = 70337,
    SPELL_NECROTIC_PLAGUE_STACK = 70338,
    SPELL_INFEST                = 70541,
    SPELL_SUMMON_GHOULS         = 70358,
    SPELL_SUMMON_HORROR         = 70372,
    SPELL_SHADOW_TRAP           = 73539,

    // Phase transition
    SPELL_REMORSELESS_WINTER_1  = 68981,
    SPELL_REMORSELESS_WINTER_2  = 72259,
    SPELL_QUAKE                 = 72262,
    SPELL_PAIN_AND_SUFFERING    = 72133,
    SPELL_RAGING_SPIRIT         = 69200,
    SPELL_SUMMON_RAGING_SPIRIT  = 69201,
    SPELL_SUMMON_ICE_SPHERE     = 69103,
    SPELL_ICE_SPHERE            = 69104, // missile and summon effect

    // Phase Two
    SPELL_SUMMON_VALKYR         = 69037,
    SPELL_SUMMON_VALKYRS        = 74361, // 25man
    SPELL_SOUL_REAPER           = 69409,
    SPELL_DEFILE                = 72762,

    // Phase Three
    SPELL_VILE_SPIRITS          = 70498,
    SPELL_HARVEST_SOUL          = 68980, // stun aura and periodic damage, triggers summoning of vehicle
    SPELL_HARVEST_SOUL_TP_FM_N  = 72546, // teleports to Frostmourne Room and applies 60sec dummy aura (normal)
    SPELL_HARVEST_SOUL_TP_FM_H  = 73655, // teleports to Frostmourne Room and applies 60sec DoT aura (heroic)
    SPELL_HARVEST_SOUL_CLONE    = 71372,
    SPELL_HARVEST_SOULS         = 74296,
    SPELL_HARVESTED_SOUL_1      = 73028,
    SPELL_HARVESTED_SOUL_2      = 74321,
    SPELL_HARVESTED_SOUL_3      = 74322,
    SPELL_HARVESTED_SOUL_4      = 74323,

    SPELL_FROSTMOURNE_TP_VISUAL = 73078,

    // Shambling Horror
    SPELL_FRENZY                = 28747,
    SPELL_ENRAGE                = 72143,
    SPELL_SHOCKWAVE             = 72149,

    // Shadow Trap
    SPELL_SHADOW_TRAP_VISUAL    = 73530,
    SPELL_SHADOW_TRAP_AURA      = 73525,

    // Raging Spirit
    SPELL_SOUL_SHRIEK           = 69242,
    SPELL_RAGING_SPIRIT_VISUAL  = 69198, // clone effect (clone of player)

    // Ice Sphere
    SPELL_ICE_SPHERE_VISUAL     = 69090,
    SPELL_ICE_BURST_AURA        = 69109,
    SPELL_ICE_BURST             = 69108,
    SPELL_ICE_PULSE             = 69091,

    // Val'kyr Shadowguard
    SPELL_LIFE_SIPHON           = 73783,
    SPELL_VALKYR_CHARGE         = 74399,
    SPELL_HARVEST_SOUL_VEHICLE  = 68985,
    SPELL_EJECT_PASSENGERS      = 68576,
    SPELL_WINGS_OF_THE_DAMNED   = 74352,

    // Defile
    SPELL_DEFILE_AURA           = 72743,
    SPELL_DEFILE_GROW           = 72756,

    // Vile Spirit and Wicked Spirit
    SPELL_SPIRIT_BURST_AURA     = 70502,

    // Spirit Warden
    SPELL_DARK_HUNGER           = 69383,
    SPELL_DESTROY_SOUL          = 74086,
    SPELL_SOUL_RIP              = 69397, // 3500, each next one x2 (maybe based on HP of target?)

    // Terenas in Frostmourne
    SPELL_RESTORE_SOUL          = 72595,
    SPELL_RESTORE_SOUL_HEROIC   = 73650,
    SPELL_LIGHTS_FAVOR          = 69382,
    SPELL_SUMMON_SPIRIT_BOMBS_1 = 73581, // heroic only, summons Spirit Bomb every 1 sec
    SPELL_SUMMON_SPIRIT_BOMBS_2 = 74299, // 2 secs interval
    SPELL_SUMMON_SPIRIT_BOMB    = 74300, // aura doesnt work somehow, so we will use manual summoning

    // Spirit Bomb
    SPELL_SPIRIT_BOMB_VISUAL    = 73572,
    SPELL_EXPLOSION             = 73804,

    // NPCs
    NPC_SHADOW_TRAP             = 39137,
    NPC_FROSTMOURNE             = 38584,
    NPC_ICE_SPHERE              = 36633,
    NPC_RAGING_SPIRIT           = 36701,
    NPC_DEFILE                  = 38757,
    NPC_SPIRIT_WARDEN           = 36824,
    NPC_TERENAS_FM_NORMAL       = 36823,
    NPC_TERENAS_FM_HEROIC       = 39217,
    NPC_WICKED_SPIRIT           = 39190,
    NPC_SPIRIT_BOMB             = 39189,
};

// talks
enum
{
    SAY_INTRO_1                 = -1631158,
    SAY_INTRO_2                 = -1631159,
    SAY_INTRO_3                 = -1631160,
    SAY_INTRO_4                 = -1631161,
    SAY_INTRO_5                 = -1631162,
    SAY_AGGRO                   = -1631163,
    SAY_REMORSELESS_WINTER      = -1631164,
    SAY_SHATTER_ARENA           = -1631165,
    SAY_SUMMON_VALKYR           = -1631166,
    SAY_HARVEST_SOUL            = -1631167,
    SAY_FM_TERENAS_AID_1        = -1631168,
    SAY_FM_TERENAS_AID_2        = -1631169,
    SAY_FM_TERENAS_AID_3        = -1631170,
    SAY_FM_PLAYER_ESCAPE        = -1631171,
    SAY_FM_PLAYER_DEATH         = -1631172,
    SAY_SPECIAL_1               = -1631173,
    SAY_SPECIAL_2               = -1631174,
    SAY_LAST_PHASE              = -1631175,
    SAY_SLAY_1                  = -1631176,
    SAY_SLAY_2                  = -1631177,
    SAY_ENRAGE                  = -1631178,
    SAY_OUTRO_1                 = -1631179,
    SAY_OUTRO_2                 = -1631180,
    SAY_OUTRO_3                 = -1631181,
    SAY_OUTRO_4                 = -1631182,
    SAY_OUTRO_5                 = -1631183,
    SAY_OUTRO_6                 = -1631184,
    SAY_OUTRO_7                 = -1631185,
    SAY_OUTRO_8                 = -1631186,
    SAY_OUTRO_9                 = -1631187,
    SAY_OUTRO_10                = -1631188,
    SAY_OUTRO_11                = -1631189,
    SAY_OUTRO_12                = -1631190,
    SAY_OUTRO_13                = -1631191,
    SAY_OUTRO_14                = -1631192,
};

enum Common
{
     FINAL_ARTHAS_MOVIE         = 16,
     EQUIP_FROSTMOURNE          = 49706,
};

enum Phase
{
    PHASE_INTRO                 = 0,    // intro
    PHASE_ONE                   = 1,    // phase one
    PHASE_RUNNING_WINTER_ONE    = 2,    // running to center of platform to cast Remorseless Winter
    PHASE_TRANSITION_ONE        = 3,    // Remorseless Winter aura and casting spells, summoning orbs and spirits
    PHASE_QUAKE_ONE             = 4,    // casting Quake
    PHASE_TWO                   = 5,    // phase two with val'kyrs and some more spells
    PHASE_RUNNING_WINTER_TWO    = 6,    // running to center of platform to cast Remorseless Winter again
    PHASE_TRANSITION_TWO        = 7,    // second Remorseless Winter phase
    PHASE_QUAKE_TWO             = 8,    // second Quake casting
    PHASE_THREE                 = 9,    // phase three, casting Soul Harvest (Frostmourne phase)
    PHASE_IN_FROSTMOURNE        = 10,   // phase three, waiting untill whole raid leaves Frostmourne
    PHASE_CUTSCENE              = 11,   // phase when LK kills raid, Terenas comes etc.
    PHASE_DEATH_AWAITS          = 12,   // strangulating Lich King, raid group finishing him
};

enum Point
{
    POINT_CENTER_LAND           = 1,
    POINT_CENTER_LAND_TIRION    = 2,
    POINT_TELEPORTER_TIRION     = 3,
    POINT_VALKYR_THROW          = 4,
    POINT_VALKYR_CENTER         = 5,
    POINT_TP_TO_FM              = 6, // point where strangulate vehicle moves, after reaching player is teleported into frostmourne
    POINT_SPIRIT_BOMB           = 7, // Spirit Bomb moving down
};

static Locations SpawnLoc[] =
{
    {458.58889f, -2122.710284f, 1040.860107f},    // 0 Lich King Intro
    {503.15652f, -2124.516602f, 1040.860107f},    // 1 Center of the platform
    {500.15652f, -2124.516602f, 1040.860107f},    // 2 Tirion strikes Lich King
    {481.69797f, -2124.638184f, 1040.860107f},    // 3 Tirion 2
    {498.00448f, -2201.573486f, 1046.093872f},    // 4 Valkyrs?
    {517.48291f, -2124.905762f, 1040.861328f},    // 5 Tirion?
    {529.85302f, -2124.709961f, 1040.859985f},    // 6 Lich king final, o=3.1146
    {520.311f,   -2124.709961f, 1040.859985f},    // 7 Frostmourne
    {453.80f,    -2088.20f,     1040.859985f},    // 8 Val'kyr drop point right to Frozen Throne
    {457.03f,    -2155.08f,     1040.859985f},    // 9 Val'kyr drop point left to Frozen Throne
    {494.313f,   -2523.079f,    1249.87f},        // 10 center of platform inside Frostmourne
};

#define GOSSIP_TIRION_START_EVENT "We are prepared, Highlord. Let us battle for the fate of Azeroth! For the light of dawn!"

/**
 * Tirion
 */
struct MANGOS_DLL_DECL boss_tirion_iccAI : public base_icc_bossAI
{
    boss_tirion_iccAI(Creature* pCreature) : base_icc_bossAI(pCreature)
    {
        m_bIsEventStarted   = false;
        m_guidTerenas.Clear();
        m_guidFrostmourne.Clear();
    }

    bool m_bIsEventStarted;
    uint32 m_uiEventStep;
    uint32 m_uiEventTimer;
    ObjectGuid m_guidTerenas;
    ObjectGuid m_guidFrostmourne;

    void Reset(){}

    void EnterEvadeMode()
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_LICH_KING) == DONE)
                base_icc_bossAI::EnterEvadeMode();
        }
    }

    void DoStartEvent()
    {
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        m_bIsEventStarted = true;
        m_uiEventStep = 0;
        m_uiEventTimer = 2000;
    }

    void DoContinueEvent()
    {
        m_uiEventTimer = 17000;
        m_uiEventStep = 10;
        m_bIsEventStarted = true;
    }

    void DoFinishEvent()
    {
        m_uiEventTimer = 0;
        m_uiEventStep = 27;
        m_bIsEventStarted = true;
    }

    void NextStep(uint32 uiTimer)
    {
        ++m_uiEventStep;
        m_uiEventTimer = uiTimer;
    }

    void DoLichKingText(int32 iText)
    {
        if (m_pInstance)
        {
            if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                DoScriptText(iText, pLichKing);
        }
    }

    void DoLichKingEmote(uint32 uiEmote)
    {
        if (m_pInstance)
        {
            if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                pLichKing->HandleEmote(uiEmote);
        }
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData)
    {
        if (uiMovementType != POINT_MOTION_TYPE)
            return;

        if (uiData == POINT_CENTER_LAND_TIRION)
        {
            m_creature->HandleEmoteCommand(EMOTE_ONESHOT_SPECIALATTACK1H);
            NextStep(1000);
        }
        else if (POINT_TELEPORTER_TIRION)
        {
            if (m_pInstance)
            {
                if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                    m_creature->SetFacingToObject(pLichKing);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bIsEventStarted)
            return;

        // events
        if (m_uiEventTimer < uiDiff)
        {
            switch(m_uiEventStep)
            {
                case 0:
                {
                    if (m_pInstance)
                    {
                        if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                        {
                            pLichKing->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
                            pLichKing->SetStandState(UNIT_STAND_STATE_STAND);
                            pLichKing->GetMotionMaster()->MovePoint(0, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, false);
                        }
                    }
                    DoLichKingText(SAY_INTRO_1);
                    NextStep(14000);
                    break;
                }
                case 1:
                {
                    DoScriptText(SAY_INTRO_2, m_creature);
                    NextStep(7000);
                    break;
                }
                case 2:
                {
                    DoLichKingText(SAY_INTRO_3);
                    NextStep(4000);
                    break;
                }
                case 3:
                {
                    DoLichKingEmote(EMOTE_ONESHOT_SHOUT);
                    NextStep(4000);
                    break;
                }
                case 4:
                {
                    DoLichKingEmote(EMOTE_ONESHOT_POINT);
                    NextStep(4000);
                    break;
                }
                case 5:
                {
                    DoLichKingEmote(EMOTE_ONESHOT_SHOUT);
                    NextStep(10000);
                    break;
                }
                case 6:
                {
                    DoScriptText(SAY_INTRO_4, m_creature);
                    m_creature->HandleEmote(EMOTE_ONESHOT_POINT);
                    NextStep(4000);
                    break;
                }
                case 7:
                {
                    if (m_pInstance)
                    {
                        if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                        {
                            m_creature->GetMotionMaster()->MoveChase(pLichKing);
                            pLichKing->CastSpell(m_creature, SPELL_ICE_LOCK, false);
                            DoScriptText(SAY_INTRO_5, pLichKing);
                        }
                    }
                    NextStep(1000);
                    break;
                }
                case 8:
                {
                    if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                    {
                        pLichKing->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                        pLichKing->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        pLichKing->SetInCombatWithZone();
                    }

                    m_bIsEventStarted = false;
                    NextStep(0);
                    break;
                }
                case 9:
                {
                    // battle with Lich King
                    break;
                }
                // *** ENDING EVENT ***
                case 10:
                {
                    DoLichKingText(SAY_OUTRO_1);
                    NextStep(26000);
                    break;
                }
                case 11:
                {
                    DoLichKingText(SAY_OUTRO_2);
                    NextStep(10000);
                    break;
                }
                case 12:
                {
                    DoLichKingText(SAY_OUTRO_3);
                    if (m_pInstance)
                    {
                        if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                            pLichKing->SetFacingToObject(m_creature);
                    }
                    NextStep(23000);
                    break;
                }
                case 13:
                {
                    if (m_pInstance)
                    {
                        if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                        {
                            DoScriptText(SAY_OUTRO_4, pLichKing);
                            pLichKing->GetMotionMaster()->MovePoint(POINT_CENTER_LAND, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z, false);
                        }
                    }
                    NextStep(27000);
                    break;
                }
                case 14:
                {
                    DoLichKingText(SAY_OUTRO_5);
                    NextStep(12000);
                    break;
                }
                case 15:
                {
                    DoScriptText(SAY_OUTRO_6, m_creature);
                    NextStep(8000);
                    break;
                }
                case 16:
                {
                    DoCastSpellIfCan(m_creature, SPELL_LIGHTS_BLESSING, CAST_TRIGGERED);
                    NextStep(5000);
                    break;
                }
                case 17:
                {
                    m_creature->RemoveAurasDueToSpell(SPELL_ICE_LOCK);

                    // run to Lich King
                    if (m_pInstance)
                    {
                        if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                        {
                            m_creature->SetWalk(false);
                            m_creature->GetMotionMaster()->MovePoint(POINT_CENTER_LAND_TIRION, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, false);
                        }
                    }

                    NextStep(0);
                    break;
                }
                case 18:
                {
                    // wait for arriving at Lich King and attack him
                    break;
                }
                case 19:
                {
                    if (m_pInstance)
                    {
                        if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                        {
                            float x, y, z;
                            pLichKing->SetVirtualItem(VIRTUAL_ITEM_SLOT_0, 0);
                            pLichKing->GetClosePoint(x, y, z, pLichKing->GetObjectBoundingRadius(), 3.0f, pLichKing->GetOrientation());
                            pLichKing->RemoveAurasDueToSpell(SPELL_RAISE_DEAD);
                            pLichKing->InterruptNonMeleeSpells(true);
                            pLichKing->CastSpell(pLichKing, SPELL_FROSTMOURNE_DESPAWN, true);
                            pLichKing->CastSpell(pLichKing, SPELL_THROW_FROSTMOURNE, false);
                            pLichKing->SetStandState(UNIT_STAND_STATE_KNEEL);
                            DoScriptText(SAY_OUTRO_7, pLichKing);

                            if (Creature *pFrostmourne = m_creature->SummonCreature(NPC_FROSTMOURNE, x, y, z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 7 * DAY * IN_MILLISECONDS))
                            {
                                pFrostmourne->CastSpell(pFrostmourne, SPELL_BROKEN_FROSTMOURNE, true);
                                m_guidFrostmourne = pFrostmourne->GetObjectGuid();
                            }
                        }
                    }

                    NextStep(5000);
                    break;
                }
                case 20:
                {
                    DoScriptText(SAY_OUTRO_8, m_creature);
                    m_creature->GetMotionMaster()->MovePoint(POINT_TELEPORTER_TIRION, SpawnLoc[6].x, SpawnLoc[6].y, SpawnLoc[6].z, false);
                    
                    if (m_pInstance)
                    {
                        if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                        {
                            if (Creature *pFrostmourne = m_pInstance->instance->GetCreature(m_guidFrostmourne))
                            {
                                float x, y, z;
                                pFrostmourne->GetPosition(x, y, z);
                                pFrostmourne->CastSpell(pFrostmourne, SPELL_FROSTMOURNE_SPIRITS, true);
                                pLichKing->CastSpell(pLichKing, SPELL_SOUL_BARRAGE, true);
                                pLichKing->SetWalk(true);
                                pLichKing->GetMotionMaster()->MovePoint(0, x, y, z + 1.0f, false);
                            }
                        }
                    }

                    NextStep(7000);
                    break;
                }
                case 21:
                {
                    if (m_pInstance)
                    {
                        if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                        {
                            float x, y, z;
                            pLichKing->GetClosePoint(x, y, z, pLichKing->GetObjectBoundingRadius(), 6.0f, pLichKing->GetAngle(m_creature) + 1.0f);

                            if (Creature *pTerenas = m_creature->SummonCreature(NPC_MENETHIL, x, y, z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 7 * DAY * IN_MILLISECONDS))
                            {
                                m_guidTerenas = pTerenas->GetObjectGuid();
                                pTerenas->SetFacingToObject(pLichKing);
                                DoScriptText(SAY_OUTRO_9, pTerenas);
                            }
                        }
                    }

                    NextStep(10000);
                    break;
                }
                case 22:
                {
                    if (m_pInstance)
                    {
                        if (Creature *pTerenas = m_pInstance->instance->GetCreature(m_guidTerenas))
                        {
                            DoScriptText(SAY_OUTRO_10, pTerenas);
                            pTerenas->CastSpell(pTerenas, SPELL_MASS_RESURRECTION, false);
                        }
                    }

                    NextStep(6000);
                    break;
                }
                case 23:
                {
                    DoScriptText(SAY_OUTRO_11, m_creature);
                    if (m_pInstance)
                    {
                        if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                        {
                            if (Creature *pTerenas = m_pInstance->instance->GetCreature(m_guidTerenas))
                                pTerenas->AI()->AttackStart(pLichKing);

                            AttackStart(pLichKing);
                        }
                    }
                    NextStep(6000);
                    break;
                }
                case 24:
                {
                    DoLichKingText(SAY_OUTRO_12);
                    NextStep(9000);
                    break;
                }
                case 25:
                {
                    DoLichKingText(SAY_OUTRO_13);
                    NextStep(0);
                    break;
                }
                case 26:
                {
                    // wait until Lich King is dead
                    break;
                }
                case 27:
                {
                    if (m_pInstance)
                    {
                        if (Creature *pFrostmourne = m_pInstance->instance->GetCreature(m_guidFrostmourne))
                            pFrostmourne->ForcedDespawn(10000);
                    }
                    NextStep(8000);
                    break;
                }
                case 28:
                {
                    // play cinematic
                    if (m_pInstance)
                    {
                        Map::PlayerList const &list = m_pInstance->instance->GetPlayers();
                        for(Map::PlayerList::const_iterator itr = list.begin(); itr != list.end(); ++itr)
                        {
                            itr->getSource()->SendMovieStart(FINAL_ARTHAS_MOVIE);
                        }
                    }
                    NextStep(0);
                    break;
                }
            }
        }
        else
            m_uiEventTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_tirion_icc(Creature* pCreature)
{
    return new boss_tirion_iccAI(pCreature);
}

bool GossipHello_boss_tirion_icc(Player* pPlayer, Creature* pCreature)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

    if (!pInstance)
        return true;

    if (pInstance->GetData(TYPE_LICH_KING) != NOT_STARTED &&
        pInstance->GetData(TYPE_LICH_KING) != FAIL )
    {
        return false;
    }

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TIRION_START_EVENT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_boss_tirion_icc(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();

        if (boss_tirion_iccAI *pTirionAI = dynamic_cast<boss_tirion_iccAI*>(pCreature->AI()))
            pTirionAI->DoStartEvent();

        return true;
    }
    else
        return false;
}


/**
 * The Lich King
 */
struct MANGOS_DLL_DECL boss_the_lich_king_iccAI : public base_icc_bossAI
{
    boss_the_lich_king_iccAI(Creature* pCreature) : base_icc_bossAI(pCreature)
    {
        Reset();
        m_uiPlatformOriginalFlag = 0;
        m_bPillarsDestroyed = false;
        m_bIsFirstAttempt = true;
    }

    uint32 m_uiPhase;
    uint32 m_uiPhaseTimer;
    uint32 m_uiBerserkTimer;
    uint32 m_uiGhoulsTimer;
    uint32 m_uiHorrorTimer;
    uint32 m_uiNecroticPlagueTimer;
    uint32 m_uiInfestTimer;
    uint32 m_uiShadowTrapTimer;
    uint32 m_uiPainSufferingTimer;
    uint32 m_uiRagingSpiritTimer;
    uint32 m_uiIceSphereTimer;
    uint32 m_uiValkyrTimer;
    uint32 m_uiDefileTimer;
    uint32 m_uiSoulReaperTimer;
    uint32 m_uiHarvestSoulTimer;
    uint32 m_uiFrostmournePhaseTimer;
    uint32 m_uiVileSpiritsTimer;

    uint32 m_uiPlatformOriginalFlag;
    uint32 m_uiRespawnPlatformTimer;
    uint32 m_uiDestroyPillarsTimer;
    bool m_bPillarsDestroyed;
    bool m_bPlatformDestroyed;
    bool m_bIsFirstAttempt;

    void Reset()
    {
        m_uiPhase               = PHASE_INTRO;
        m_uiBerserkTimer        = 15 * MINUTE * IN_MILLISECONDS;
        m_uiGhoulsTimer         = 13000;
        m_uiHorrorTimer         = 21000;
        m_uiInfestTimer         = 20000;
        m_uiNecroticPlagueTimer = 23000;
        m_uiShadowTrapTimer     = 15000;
        m_uiPainSufferingTimer  = 6000;
        m_uiRagingSpiritTimer   = 20000;
        m_uiIceSphereTimer      = 6000;
        m_uiValkyrTimer         = 10000;
        m_uiDefileTimer         = 30000;
        m_uiSoulReaperTimer     = 25000;
        m_uiHarvestSoulTimer    = 5000;
        m_uiVileSpiritsTimer    = 20000;
        m_uiDestroyPillarsTimer = 3000;
        m_uiRespawnPlatformTimer= 3000;

        DoRespawnPlatform();
        SetCombatMovement(true);
    }

    void Aggro(Unit *pWho)
    {
        if (!m_bIsFirstAttempt)
            DoScriptText(SAY_AGGRO, m_creature); // say aggro if this is another attempt

        m_uiPhase = PHASE_ONE;

        m_creature->SetWalk(false);
        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->RemoveAurasDueToSpell(SPELL_SIT_EMOTE_NO_SHEATH);
        SetEquipmentSlots(false, EQUIP_FROSTMOURNE);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_LICH_KING, IN_PROGRESS);
    }

    void KilledUnit(Unit *pWho)
    {
        if (pWho->GetTypeId() == TYPEID_PLAYER && m_uiPhase != PHASE_DEATH_AWAITS)
            DoScriptText(SAY_SLAY_1 - urand(0, 1), m_creature);
    }

    void JustDied(Unit *pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LICH_KING, DONE);

        DoScriptText(SAY_OUTRO_14, m_creature);

        // finish event, after around 8 seconds play cinematic
        if (m_pInstance)
        {
            if (Creature *pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_TIRION))
            {
                if (boss_tirion_iccAI *pTirionAI = dynamic_cast<boss_tirion_iccAI*>(pTirion->AI()))
                    pTirionAI->DoFinishEvent();
            }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_LICH_KING, FAIL);
            m_pInstance->SetData(TYPE_FROSTMOURNE_ROOM, DONE);
        }

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SIT);
        m_creature->SetStandState(UNIT_STAND_STATE_SIT);
        DoCastSpellIfCan(m_creature, SPELL_SIT_EMOTE_NO_SHEATH, CAST_TRIGGERED);
        m_creature->SetVirtualItem(VIRTUAL_ITEM_SLOT_0, 0);

        m_bIsFirstAttempt = false;
    }

    void JustSummoned(Creature *pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_ICE_SPHERE)
        {
            pSummoned->SetInCombatWithZone();

            if (Unit *pTarget = SelectTargetForIceSphere())
            {
                pSummoned->AddThreat(pTarget, 100000.0f);
                pSummoned->AI()->AttackStart(pTarget);
                pSummoned->CastSpell(pSummoned, SPELL_ICE_BURST_AURA, true);
                pSummoned->CastSpell(pTarget, SPELL_ICE_PULSE, false);
            }
        }
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData)
    {
        if (uiMovementType != POINT_MOTION_TYPE)
            return;

        switch(uiData)
        {
            case POINT_CENTER_LAND:
            {
                if (m_uiPhase == PHASE_RUNNING_WINTER_ONE)
                {
                    DoScriptText(SAY_REMORSELESS_WINTER, m_creature);
                    DoCastSpellIfCan(m_creature, SPELL_REMORSELESS_WINTER_1);
                    m_uiPhase = PHASE_TRANSITION_ONE;
                    m_uiPhaseTimer          = 62000;
                    // set phase initial timers
                    m_uiPainSufferingTimer  = 6000;
                    m_uiRagingSpiritTimer   = 20000;
                    m_uiIceSphereTimer      = 6000;

                    // on heroic despawn Shadow Traps
                    if (m_bIsHeroic)
                        DoDespawnShadowTraps();
                }
                else if (m_uiPhase == PHASE_RUNNING_WINTER_TWO)
                {
                    DoScriptText(SAY_REMORSELESS_WINTER, m_creature);
                    DoCastSpellIfCan(m_creature, SPELL_REMORSELESS_WINTER_2);
                    m_uiRespawnPlatformTimer = 3000; // respawn platform after spell is cast
                    m_uiPhase = PHASE_TRANSITION_TWO;
                    m_uiPhaseTimer          = 62000;
                    // set phase initial timers
                    m_uiPainSufferingTimer  = 6000;
                    m_uiRagingSpiritTimer   = 15000;
                    m_uiIceSphereTimer      = 6000;
                }
                else if (m_uiPhase == PHASE_DEATH_AWAITS)
                {
                    DoCastSpellIfCan(m_creature, SPELL_RAISE_DEAD);
                }

                break;
            }
        }
    }

    void DoDestroyPillars()
    {
        if (m_pInstance)
        {
            if (GameObject *pGo = m_pInstance->GetSingleGameObjectFromStorage(GO_ICESHARD_1))
                pGo->SetGoState(GO_STATE_ACTIVE);
            if (GameObject *pGo = m_pInstance->GetSingleGameObjectFromStorage(GO_ICESHARD_2))
                pGo->SetGoState(GO_STATE_ACTIVE);
            if (GameObject *pGo = m_pInstance->GetSingleGameObjectFromStorage(GO_ICESHARD_3))
                pGo->SetGoState(GO_STATE_ACTIVE);
            if (GameObject *pGo = m_pInstance->GetSingleGameObjectFromStorage(GO_ICESHARD_4))
                pGo->SetGoState(GO_STATE_ACTIVE);
        }
    }

    void DoDestroyPlatform()
    {
        if (!m_pInstance)
            return;

        if (GameObject* pGoFloor = m_pInstance->GetSingleGameObjectFromStorage(GO_ARTHAS_PLATFORM))
        {
             pGoFloor->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_DAMAGED | GO_FLAG_NODESPAWN);
             m_uiPlatformOriginalFlag = pGoFloor->GetUInt32Value(GAMEOBJECT_BYTES_1);
             pGoFloor->SetUInt32Value(GAMEOBJECT_BYTES_1, 8449);
        }

        if (GameObject *pGo = m_pInstance->GetSingleGameObjectFromStorage(GO_FROSTY_WIND))
            pGo->SetGoState(GO_STATE_READY);
        if (GameObject *pGo = m_pInstance->GetSingleGameObjectFromStorage(GO_SNOW_EDGE))
            pGo->SetGoState(GO_STATE_READY);

        m_bPlatformDestroyed = true;
    }

    void DoRespawnPlatform()
    {
        if (!m_pInstance)
            return;

        if (GameObject* pGoFloor = m_pInstance->GetSingleGameObjectFromStorage(GO_ARTHAS_PLATFORM))
        {
            pGoFloor->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_DAMAGED | GO_FLAG_NODESPAWN);
            pGoFloor->SetUInt32Value(GAMEOBJECT_BYTES_1, m_uiPlatformOriginalFlag);
        }

        if (GameObject* pGo = m_pInstance->GetSingleGameObjectFromStorage(GO_FROSTY_WIND))
            pGo->SetGoState(GO_STATE_ACTIVE);

        m_bPlatformDestroyed = false;
    }

    void DoDespawnShadowTraps()
    {
        std::list<Creature*> lTraps;
        GetCreatureListWithEntryInGrid(lTraps, m_creature, NPC_SHADOW_TRAP, 120.0f);
        if (!lTraps.empty())
        {
            for(std::list<Creature*>::iterator i = lTraps.begin(); i != lTraps.end();)
            {
                std::list<Creature*>::iterator next = i;
                ++next;
                if (*i)
                    (*i)->ForcedDespawn();

                i = next;
            }
        }
    }

    Unit* SelectTargetForIceSphere()
    {
        std::list<ObjectGuid> lPotentialTargets;
        const ThreatList &threatList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = threatList.begin();itr != threatList.end(); ++itr)
        {
            if (Unit *pVictim = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
            {
                if (!pVictim->HasAuraOfDifficulty(SPELL_ICE_PULSE))
                    lPotentialTargets.push_back((*itr)->getUnitGuid());
            }
        }

        if (!lPotentialTargets.empty())
        {
            std::list<ObjectGuid>::iterator i = lPotentialTargets.begin();
            std::advance(i, urand(0, lPotentialTargets.size() - 1));
            return m_creature->GetMap()->GetUnit(*i);
        }
        else
            return m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_ICE_PULSE, SELECT_FLAG_PLAYER);
    }

    Unit* SelectTargetForNecroticPlague()
    {
        std::list<ObjectGuid> lPotentialTargets;
        const ThreatList &threatList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = threatList.begin();itr != threatList.end(); ++itr)
        {
            if (Unit *pVictim = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
            {
                if (pVictim != m_creature->getVictim() &&
                    !pVictim->HasAuraOfDifficulty(SPELL_NECROTIC_PLAGUE) &&
                    !pVictim->HasAuraOfDifficulty(SPELL_NECROTIC_PLAGUE_STACK))
                {
                    lPotentialTargets.push_back((*itr)->getUnitGuid());
                }
            }
        }

        if (!lPotentialTargets.empty())
        {
            std::list<ObjectGuid>::iterator i = lPotentialTargets.begin();
            std::advance(i, urand(0, lPotentialTargets.size() - 1));
            return m_creature->GetMap()->GetUnit(*i);
        }
        else
            return NULL;
    }

    Unit* SelectTargetForDefile()
    {
        std::list<ObjectGuid> lPotentialTargets;
        const ThreatList &threatList = m_creature->getThreatManager().getThreatList();
        for (ThreatList::const_iterator itr = threatList.begin();itr != threatList.end(); ++itr)
        {
            if (Unit *pVictim = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
            {
                if (pVictim->GetTypeId() == TYPEID_PLAYER && !pVictim->GetVehicle())
                    lPotentialTargets.push_back((*itr)->getUnitGuid());
            }
        }

        if (!lPotentialTargets.empty())
        {
            std::list<ObjectGuid>::iterator i = lPotentialTargets.begin();
            std::advance(i, urand(0, lPotentialTargets.size() - 1));
            return m_creature->GetMap()->GetUnit(*i);
        }
        else
            return m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_DEFILE, SELECT_FLAG_PLAYER);
    }

    void DoPrepareFrostmourneRoom()
    {
        // Terenas + Spirit Warden
        if (!m_bIsHeroic)
        {
            m_creature->SummonCreature(NPC_TERENAS_FM_NORMAL, SpawnLoc[10].x - 5.0f, SpawnLoc[10].y, SpawnLoc[10].z, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
            m_creature->SummonCreature(NPC_SPIRIT_WARDEN, SpawnLoc[10].x + 5.0f, SpawnLoc[10].y, SpawnLoc[10].z, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
        }
        // Terenas + Spirit Bombs and Spirits
        else
        {
            m_creature->SummonCreature(NPC_TERENAS_FM_HEROIC, SpawnLoc[10].x, SpawnLoc[10].y, SpawnLoc[10].z, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FROSTMOURNE_ROOM, IN_PROGRESS);
    }

    void SpellHit(Unit *pCaster, const SpellEntry *pSpell)
    {
        if (pSpell->Id == SPELL_HARVESTED_SOUL_1 ||
            pSpell->Id == SPELL_HARVESTED_SOUL_2 ||
            pSpell->Id == SPELL_HARVESTED_SOUL_3 ||
            pSpell->Id == SPELL_HARVESTED_SOUL_4)
        {
            DoScriptText(SAY_FM_PLAYER_DEATH, m_creature);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiPhase != PHASE_INTRO && m_uiPhase != PHASE_DEATH_AWAITS)
        {
            // check evade
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            // Berserk
            if (m_uiBerserkTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                {
                    DoScriptText(SAY_ENRAGE, m_creature);
                    m_uiBerserkTimer = 30 * MINUTE * IN_MILLISECONDS;
                }
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        switch(m_uiPhase)
        {
            case PHASE_INTRO:
            {
                // wait until set in combat
                return;
            }
            case PHASE_ONE:
            {
                // check HP
                if (m_creature->GetHealthPercent() <= 70.0f)
                {
                    // phase transition
                    SetCombatMovement(false);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MovePoint(POINT_CENTER_LAND, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                    m_uiPhase = PHASE_RUNNING_WINTER_ONE;
                    return;
                }

                // Necrotic Plague
                if (m_uiNecroticPlagueTimer < uiDiff)
                {
                    if (Unit *pTarget = SelectTargetForNecroticPlague())
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_NECROTIC_PLAGUE) == CAST_OK)
                            m_uiNecroticPlagueTimer = 30000;
                    }
                }
                else
                    m_uiNecroticPlagueTimer -= uiDiff;

                // Infest
                if (m_uiInfestTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_INFEST) == CAST_OK)
                        m_uiInfestTimer = urand(20000, 25000);
                }
                else
                    m_uiInfestTimer -= uiDiff;

                // Summon Ghouls
                if (m_uiGhoulsTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_GHOULS) == CAST_OK)
                        m_uiGhoulsTimer = 32000;
                }
                else
                    m_uiGhoulsTimer -= uiDiff;

                // Summon Shambling Horror
                if (m_uiHorrorTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_HORROR) == CAST_OK)
                        m_uiHorrorTimer = 60000;
                }
                else
                    m_uiHorrorTimer -= uiDiff;

                // Shadow Trap (heroic)
                if (m_bIsHeroic)
                {
                    if (m_uiShadowTrapTimer < uiDiff)
                    {
                        if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_SHADOW_TRAP, SELECT_FLAG_PLAYER))
                        {
                            if (DoCastSpellIfCan(pTarget, SPELL_SHADOW_TRAP) == CAST_OK)
                                m_uiShadowTrapTimer = 15000;
                        }
                    }
                    else
                        m_uiShadowTrapTimer -= uiDiff;
                }

                DoMeleeAttackIfReady();
                break;
            }
            case PHASE_RUNNING_WINTER_ONE:
            case PHASE_RUNNING_WINTER_TWO:
            {
                // wait for waypoint arrival
                break;
            }
            case PHASE_TRANSITION_ONE:
            case PHASE_TRANSITION_TWO:
            {
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;

                // phase end timer
                if (m_uiPhaseTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_QUAKE) == CAST_OK)
                    {
                        DoScriptText(SAY_SHATTER_ARENA, m_creature);
                        m_uiPhase = (m_uiPhase == PHASE_TRANSITION_ONE ? PHASE_QUAKE_ONE : PHASE_QUAKE_TWO);
                        m_uiPhaseTimer = 6500;
                    }
                }
                else
                    m_uiPhaseTimer -= uiDiff;

                // destroy pillars
                if (!m_bPillarsDestroyed)
                {
                    if (m_uiDestroyPillarsTimer < uiDiff)
                    {
                        DoDestroyPillars();
                        m_bPillarsDestroyed = true;
                    }
                    else
                        m_uiDestroyPillarsTimer -= uiDiff;
                }

                // respawning platform
                if (m_bPlatformDestroyed)
                {
                    if (m_uiRespawnPlatformTimer < uiDiff)
                    {
                        DoRespawnPlatform();
                    }
                    else
                        m_uiRespawnPlatformTimer -= uiDiff;
                }

                // Pain and Suffering
                if (m_uiPainSufferingTimer < uiDiff)
                {
                    if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_PAIN_AND_SUFFERING, SELECT_FLAG_PLAYER))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_PAIN_AND_SUFFERING, CAST_TRIGGERED) == CAST_OK)
                            m_uiPainSufferingTimer = urand(1500, 3000);
                    }
                }
                else
                    m_uiPainSufferingTimer -= uiDiff;

                // Summon Ice Sphere
                if (m_uiIceSphereTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_ICE_SPHERE) == CAST_OK)
                        m_uiIceSphereTimer = 6000;
                }
                else
                    m_uiIceSphereTimer -= uiDiff;

                // Summon Raging Spirit
                if (m_uiRagingSpiritTimer < uiDiff)
                {
                    if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_RAGING_SPIRIT, SELECT_FLAG_PLAYER))
                    {
                        // some targeting issues with script effect spell
                        // but we dont need it in sd2 ;)
                        // if (DoCastSpellIfCan(m_creature, SPELL_RAGING_SPIRIT, CAST_TRIGGERED) == CAST_OK)

                        pTarget->CastSpell(pTarget, SPELL_SUMMON_RAGING_SPIRIT, true);
                        m_uiRagingSpiritTimer = (m_uiPhase == PHASE_TRANSITION_ONE ? 20000 : 15000);
                    }
                }
                else
                    m_uiRagingSpiritTimer -= uiDiff;

                break;
            }
            case PHASE_QUAKE_ONE:
            case PHASE_QUAKE_TWO:
            {
                // Casting Quake spell - phase end timer
                if (m_uiPhaseTimer < uiDiff)
                {
                    DoDestroyPlatform();
                    m_uiPhase = (m_uiPhase == PHASE_QUAKE_ONE ? PHASE_TWO : PHASE_THREE);
                    SetCombatMovement(true);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    return;
                }
                else
                    m_uiPhaseTimer -= uiDiff;

                break;
            }
            case PHASE_TWO:
            {
                // check HP
                if (m_creature->GetHealthPercent() <= 40.0f)
                {
                    // phase transition
                    SetCombatMovement(false);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MovePoint(POINT_CENTER_LAND, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z);
                    m_uiPhaseTimer = 60000;
                    m_uiPhase = PHASE_RUNNING_WINTER_TWO;
                }

                // Soul Reaper
                if (m_uiSoulReaperTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SOUL_REAPER) == CAST_OK)
                        m_uiSoulReaperTimer = 30000;
                }
                else
                    m_uiSoulReaperTimer -= uiDiff;

                // Infest
                if (m_uiInfestTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_INFEST) == CAST_OK)
                        m_uiInfestTimer = urand(20000, 25000);
                }
                else
                    m_uiInfestTimer -= uiDiff;

                // Defile
                if (m_uiDefileTimer < uiDiff)
                {
                    if (Unit *pTarget = SelectTargetForDefile())
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_DEFILE) == CAST_OK)
                            m_uiDefileTimer = 30000;
                    }
                }
                else
                    m_uiDefileTimer -= uiDiff;

                // Summon Val'kyr
                if (m_uiValkyrTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_VALKYR) == CAST_OK)
                    {
                        DoScriptText(SAY_SUMMON_VALKYR, m_creature);
                        m_uiValkyrTimer = 50000;
                    }
                }
                else
                    m_uiValkyrTimer -= uiDiff;

                DoMeleeAttackIfReady();
                break;
            }
            case PHASE_THREE:
            {
                // check HP
                if (m_creature->GetHealthPercent() <= 10.0f)
                {
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->StopMoving();
                    SetCombatMovement(false);
                    DoCastSpellIfCan(m_creature, SPELL_FURY_OF_FROSTMOURNE);
                    DoScriptText(SAY_LAST_PHASE, m_creature);
                    m_uiPhase = PHASE_DEATH_AWAITS;

                    // tell Tirion to continue the event
                    if (m_pInstance)
                    {
                        if (Creature *pTirion = m_pInstance->GetSingleCreatureFromStorage(NPC_TIRION))
                        {
                            if (boss_tirion_iccAI *pTirionAI = dynamic_cast<boss_tirion_iccAI*>(pTirion->AI()))
                                pTirionAI->DoContinueEvent();
                        }
                    }

                    return;
                }

                // Soul Reaper
                if (m_uiSoulReaperTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SOUL_REAPER) == CAST_OK)
                        m_uiSoulReaperTimer = 30000;
                }
                else
                    m_uiSoulReaperTimer -= uiDiff;

                // Defile
                if (m_uiDefileTimer < uiDiff)
                {
                    if (Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_DEFILE, SELECT_FLAG_PLAYER))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_DEFILE) == CAST_OK)
                            m_uiDefileTimer = 30000;
                    }
                }
                else
                    m_uiDefileTimer -= uiDiff;

                // Harvest Soul
                if (m_uiHarvestSoulTimer < uiDiff)
                {
                    Unit *pTarget = NULL;
                    if (m_bIsHeroic)
                        pTarget = m_creature;
                    else
                        pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, SPELL_HARVEST_SOUL, SELECT_FLAG_PLAYER);

                    if (pTarget)
                    {
                        if (DoCastSpellIfCan(pTarget, m_bIsHeroic ? SPELL_HARVEST_SOULS : SPELL_HARVEST_SOUL) == CAST_OK)
                        {
                            DoScriptText(SAY_HARVEST_SOUL, m_creature);
                            m_uiHarvestSoulTimer = m_bIsHeroic ? 120000 : 70000;
                            DoPrepareFrostmourneRoom();

                            if (m_bIsHeroic)
                            {
                                m_uiPhase = PHASE_IN_FROSTMOURNE;
                                SetCombatMovement(false);
                                m_creature->StopMoving();
                                m_creature->GetMotionMaster()->Clear();
                                m_uiFrostmournePhaseTimer = 47000;
                                m_uiDefileTimer = 1000;
                            }
                        }
                    }
                }
                else
                    m_uiHarvestSoulTimer -= uiDiff;

                // Vile Spirits (start moving after 15seconds)
                if (m_uiVileSpiritsTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_VILE_SPIRITS) == CAST_OK)
                        m_uiVileSpiritsTimer = 30000;
                }
                else
                    m_uiVileSpiritsTimer -= uiDiff;

                DoMeleeAttackIfReady();
                break;
            }
            case PHASE_IN_FROSTMOURNE:
            {
                // check if players are alive before entering evade mode?
                // wait until they leave Frostmourne
                if (m_uiFrostmournePhaseTimer < uiDiff)
                {
                    m_uiPhase = PHASE_THREE;
                    if (m_creature->getVictim())
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    return;
                }
                else
                    m_uiFrostmournePhaseTimer -= uiDiff;

                break;
            }
            case PHASE_DEATH_AWAITS:
            {
                // wait for swift death
                break;
            }
        }
    }
};

CreatureAI* GetAI_boss_the_lich_king_icc(Creature* pCreature)
{
    return new boss_the_lich_king_iccAI(pCreature);
}

/**
 * Drudge Ghoul
 */
struct MANGOS_DLL_DECL mob_drudge_ghoulAI : public base_icc_bossAI
{
    mob_drudge_ghoulAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiReadyTimer;
    bool m_bIsReady;
    bool m_bIsSpawned;

    void Reset()
    {
        m_bIsReady          = false;
        m_bIsSpawned        = false;
        m_uiReadyTimer      = 5000;
    }

    void JustReachedHome()
    {
        m_creature->ForcedDespawn();
    }

    void AttackStart(Unit *pWho)
    {
        if (!m_bIsReady)
            return;
        else
            base_icc_bossAI::AttackStart(pWho);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bIsReady)
        {
            if (!m_bIsSpawned)
            {
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
                m_bIsSpawned = true;
            }

            if (m_uiReadyTimer <= uiDiff)
            {
                m_bIsReady = true;
                if (m_creature->getVictim())
                    AttackStart(m_creature->getVictim());
                else
                    m_creature->SetInCombatWithZone();
            }
            else
                m_uiReadyTimer -= uiDiff;

            return;
        }

        if (!m_pInstance || m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_drudge_ghoul(Creature* pCreature)
{
    return new mob_drudge_ghoulAI(pCreature);
}

/**
 * Shambling Horror
 */
struct MANGOS_DLL_DECL mob_shambling_horrorAI : public base_icc_bossAI
{
    mob_shambling_horrorAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiShockwaveTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiReadyTimer;
    bool m_bHasCastFrenzy;
    bool m_bIsReady;
    bool m_bIsSpawned;

    void Reset()
    {
        m_bIsReady          = false;
        m_bIsSpawned        = false;
        m_uiReadyTimer      = 5000;

        m_uiShockwaveTimer  = 12000;
        m_uiEnrageTimer     = 6000;
        m_bHasCastFrenzy    = false;
    }

    void JustReachedHome()
    {
        m_creature->ForcedDespawn();
    }

    void AttackStart(Unit *pWho)
    {
        if (!m_bIsReady)
            return;
        else
            base_icc_bossAI::AttackStart(pWho);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bIsReady)
        {
            if (!m_bIsSpawned)
            {
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_EMERGE);
                m_bIsSpawned = true;
            }

            if (m_uiReadyTimer <= uiDiff)
            {
                m_bIsReady = true;
                if (m_creature->getVictim())
                    AttackStart(m_creature->getVictim());
                else
                    m_creature->SetInCombatWithZone();
            }
            else
                m_uiReadyTimer -= uiDiff;

            return;
        }

        if (!m_pInstance || m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Frenzy on heroic
        if (m_bIsHeroic)
        {
            if (!m_bHasCastFrenzy)
            {
                if (m_creature->GetHealthPercent() <= 20.0f)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_FRENZY) == CAST_OK)
                        m_bHasCastFrenzy = true;
                }
            }
        }

        // Enrage
        if (m_uiEnrageTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
            {
                m_uiEnrageTimer = 20000;
                m_uiShockwaveTimer = urand(5000, 7000);
            }
        }
        else
            m_uiEnrageTimer -= uiDiff;

        // Shockwave
        if (m_uiShockwaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHOCKWAVE) == CAST_OK)
                m_uiShockwaveTimer = 20000;
        }
        else
            m_uiShockwaveTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_shambling_horror(Creature* pCreature)
{
    return new mob_shambling_horrorAI(pCreature);
}

/**
 * Shadow Trap
 */
struct MANGOS_DLL_DECL mob_shadow_trapAI : public base_icc_bossAI
{
    mob_shadow_trapAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        SetCombatMovement(false);
        DoCastSpellIfCan(m_creature, SPELL_SHADOW_TRAP_VISUAL, CAST_TRIGGERED);
        m_uiShadowTrapTimer = 5000;
        m_bHasCastAura = false;
    }

    uint32 m_uiShadowTrapTimer;
    bool m_bHasCastAura;

    void Reset(){}

    void DamageTaken(Unit *pDealer, uint32 &uiDamage)
    {
        uiDamage = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        // Shadow Trap
        if (!m_bHasCastAura)
        {
            if (m_uiShadowTrapTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SHADOW_TRAP_AURA) == CAST_OK)
                {
                    m_creature->RemoveAurasDueToSpell(SPELL_SHADOW_TRAP_VISUAL);
                    m_bHasCastAura = true;
                }
            }
            else
                m_uiShadowTrapTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_mob_shadow_trap(Creature* pCreature)
{
    return new mob_shadow_trapAI(pCreature);
}

/**
 * Ice Sphere
 */
struct MANGOS_DLL_DECL mob_ice_sphere_iccAI : public base_icc_bossAI
{
    mob_ice_sphere_iccAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        m_creature->SetWalk(true);
        m_creature->SetSpeedRate(MOVE_WALK, 1.1f);
    }

    void Reset(){}

    void JustDied(Unit *pKiller)
    {
        m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_mob_ice_sphere_icc(Creature* pCreature)
{
    return new mob_ice_sphere_iccAI(pCreature);
}

/**
 * Raging Spirit
 */
struct MANGOS_DLL_DECL mob_raging_spiritAI : public base_icc_bossAI
{
    mob_raging_spiritAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiSoulShriekTimer;
    bool m_bHasCloned;

    void Reset()
    {
        m_uiSoulShriekTimer = 8000;
        m_bHasCloned = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bHasCloned)
        {
            if (Unit *pCreator = m_creature->GetCreator())
            {
                pCreator->CastSpell(m_creature, SPELL_RAGING_SPIRIT_VISUAL, true);
                m_bHasCloned = true;
            }
        }

        // Soul Shriek
        if (m_uiSoulShriekTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SOUL_SHRIEK) == CAST_OK)
                m_uiSoulShriekTimer = 10000;
        }
        else
            m_uiSoulShriekTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_raging_spirit(Creature* pCreature)
{
    return new mob_raging_spiritAI(pCreature);
}

/**
 * Val'kyr Shadowguard
 */
struct MANGOS_DLL_DECL mob_valkyr_shadowguardAI : public base_icc_bossAI
{
    mob_valkyr_shadowguardAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        Reset();
        DoCastSpellIfCan(m_creature, SPELL_WINGS_OF_THE_DAMNED, CAST_TRIGGERED);
        SetCombatMovement(false);
        m_creature->SetLevitate(true);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
        m_creature->NearTeleportTo(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 15.0f, 0.0f);
    }

    uint32 m_uiCatchTimer;
    uint32 m_uiLifeSiphonTimer;
    bool m_bIsReady;
    bool m_bIsCarrying;
    bool m_bIsCastingLifeSiphon;

    void Reset()
    {
        m_bIsReady = false;
        m_bIsCarrying = true;
        m_bIsCastingLifeSiphon = false;
        m_uiCatchTimer = 2000;
        m_uiLifeSiphonTimer = 2000;
    }

    void AttackStart(Unit *pWho)
    {
        if (m_bIsCastingLifeSiphon)
            base_icc_bossAI::AttackStart(pWho);
    }

    Unit* DoCatchTarget()
    {
        if (m_pInstance)
        {
            if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
            {
                // search for proper target
                std::list<ObjectGuid> lPotentialTargets;
                ThreatList const &lThreatList = pLichKing->getThreatManager().getThreatList();

                if (!lThreatList.empty())
                {
                    for (ThreatList::const_iterator i = lThreatList.begin(); i != lThreatList.end(); ++i)
                    {
                        if (Unit *pTmp = m_pInstance->instance->GetUnit((*i)->getUnitGuid()))
                        {
                            if (pTmp != pLichKing->getVictim() &&
                                pTmp->GetTypeId() == TYPEID_PLAYER &&
                                !pTmp->GetVehicle())
                            {
                                lPotentialTargets.push_back(pTmp->GetObjectGuid());
                            }
                        }
                    }
                }

                // select random target
                if (!lPotentialTargets.empty())
                {
                    std::list<ObjectGuid>::iterator i = lPotentialTargets.begin();
                    std::advance(i, urand(0, lPotentialTargets.size() - 1));

                    if (Unit *pTarget = m_pInstance->instance->GetUnit(*i))
                    {
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                        // charge isn't properly implemented...
                        DoCastSpellIfCan(pTarget, SPELL_VALKYR_CHARGE, CAST_TRIGGERED);
                        m_creature->NearTeleportTo(pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ() + 3.0f, 0.0f);
                        pTarget->CastSpell(m_creature, SPELL_HARVEST_SOUL_VEHICLE, true);
                        m_bIsCarrying = true;
                        return pTarget;
                    }
                }
            }
        }

        return NULL;
    }

    void DoFlyAway(Unit *pTarget)
    {
        float curX, curY, curZ;
        float destX, destY, destZ;
        pTarget->GetPosition(curX, curY, curZ);

        // don't fly into Frozen Throne
        if (curX < 480.0f)
        {
            if (curY > -2124.0f)
                m_creature->GetMotionMaster()->MovePoint(POINT_VALKYR_THROW, SpawnLoc[8].x, SpawnLoc[8].y, SpawnLoc[8].z + 3.0f, false);
            else
                m_creature->GetMotionMaster()->MovePoint(POINT_VALKYR_THROW, SpawnLoc[9].x, SpawnLoc[9].y, SpawnLoc[9].z + 3.0f, false);

            return;
        }

        // angle to center
        float angle = m_creature->GetAngle(SpawnLoc[1].x, SpawnLoc[1].y);
        // angle away from center
        angle += M_PI_F;
        if (angle > 2 * M_PI_F)
            angle -= (2 * M_PI_F);

        // don't fly into Frozen Throne
        if (angle < 3.8f && angle > 2.3f)
        {
            if (angle > 3.05f)
                angle = 3.8f;
            else
                angle = 2.3f;
        }

        float fDistToCenter = m_creature->GetDistance2d(SpawnLoc[1].x, SpawnLoc[1].y);
        float fDist = 60.0f - fDistToCenter;

        m_creature->GetNearPoint(m_creature, destX, destY, destZ, 0.0f, fDist, angle);
        m_creature->GetMotionMaster()->MovePoint(POINT_VALKYR_THROW, destX, destY, SpawnLoc[1].z + 3.0f, false);
    }

    void DoThrowAndGoBack()
    {
        DoCastSpellIfCan(m_creature, SPELL_EJECT_PASSENGERS, CAST_TRIGGERED);
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MovePoint(POINT_VALKYR_CENTER, SpawnLoc[1].x + frand(-3.0f, 3.0f), SpawnLoc[1].y + frand(-3.0f, 3.0f), SpawnLoc[1].z + 12.0f, false);
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData)
    {
        if (uiMovementType != POINT_MOTION_TYPE)
            return;

        if (uiData == POINT_VALKYR_THROW)
        {
            DoThrowAndGoBack();
        }
        else if (uiData == POINT_VALKYR_CENTER)
        {
            if (m_bIsHeroic)
            {
                // start casting Life Siphon
                if (m_creature->GetHealthPercent() <= 50.0f)
                    m_bIsCarrying = false;
                else
                    m_bIsReady = false;
            }
            else
            {
                // catch another target
                m_bIsReady = false;
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // on heroic start casting Life Siphon at 50% HP
        if (m_bIsHeroic && !m_bIsCastingLifeSiphon)
        {
            if (m_creature->GetHealthPercent() <= 50.0f)
            {
                m_bIsReady = true;
                m_bIsCarrying = true;
                m_bIsCastingLifeSiphon = true;
                DoThrowAndGoBack();
            }
        }

        if (!m_bIsReady)
        {
            if (m_uiCatchTimer < uiDiff)
            {
                if (Unit *pTarget = DoCatchTarget())
                {
                    m_bIsReady = true;
                    DoFlyAway(pTarget);
                }
            }
            else
                m_uiCatchTimer -= uiDiff;

            return;
        }

        if (!m_pInstance || m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsHeroic && !m_bIsCarrying)
        {
            // Life Siphon
            if (m_uiLifeSiphonTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_LIFE_SIPHON) == CAST_OK)
                    m_uiLifeSiphonTimer = 5000;
            }
            else
                m_uiLifeSiphonTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_mob_valkyr_shadowguard(Creature* pCreature)
{
    return new mob_valkyr_shadowguardAI(pCreature);
}

/**
 * Defile
 */
struct MANGOS_DLL_DECL mob_defiler_iccAI : public base_icc_bossAI
{
    mob_defiler_iccAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        SetCombatMovement(false);
        DoCastSpellIfCan(m_creature, SPELL_DEFILE_AURA, CAST_TRIGGERED);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED); // don't turn around
        // "lasts 30 seconds"
        m_creature->ForcedDespawn(30000);
    }

    void Reset(){}
    void AttackStart(Unit *pWho){}
    void UpdateAI(const uint32 uiDiff){}
};

CreatureAI* GetAI_mob_defiler_icc(Creature* pCreature)
{
    return new mob_defiler_iccAI(pCreature);
}

/**
 * Vile Spirit
 */
struct MANGOS_DLL_DECL  mob_vile_spiritAI : public base_icc_bossAI
{
    mob_vile_spiritAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        SetCombatMovement(false);
        m_creature->SetLevitate(true);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
        m_creature->SetInCombatWithZone();
        m_creature->GetMotionMaster()->Clear();
        m_creature->NearTeleportTo(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 15.0f, 0.0f);
        m_creature->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX() + frand(-3.0f, 3.0f), m_creature->GetPositionY() + frand(-3.0f, 3.0f), m_creature->GetPositionZ(), false);
        DoCastSpellIfCan(m_creature, SPELL_SPIRIT_BURST_AURA, CAST_TRIGGERED);

        m_bIsWickedSpirit = m_creature->GetEntry() == NPC_WICKED_SPIRIT;

        if (m_bIsWickedSpirit)
            m_creature->ForcedDespawn(41000);

        Reset();
    }

    bool m_bIsReady;
    bool m_bIsWickedSpirit;
    uint32 m_uiReadyTimer;
    uint32 m_uiSummonSpiritBombTimer;

    void Reset()
    {
        m_bIsReady = false;
        m_uiReadyTimer = m_bIsWickedSpirit ? urand(7000, 15000) : 15000;
        m_uiSummonSpiritBombTimer = urand(1000, 10000);
    }

    void MovementInform(uint32 uiMovementType, uint32 uiData)
    {
        if (uiMovementType != POINT_MOTION_TYPE)
            return;

        if (m_bIsReady)
            return;

        if (uiData == 1)
            m_creature->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX() + frand(-3.0f, 3.0f), m_creature->GetPositionY() + frand(-3.0f, 3.0f), m_creature->GetPositionZ(), false);
    }

    Unit* SelectTarget()
    {
        if (m_pInstance)
        {
            if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
            {
                // search for proper target
                std::list<ObjectGuid> lPotentialTargets;
                ThreatList const &lThreatList = pLichKing->getThreatManager().getThreatList();

                if (!lThreatList.empty())
                {
                    for (ThreatList::const_iterator i = lThreatList.begin(); i != lThreatList.end(); ++i)
                    {
                        if (Unit *pTmp = m_pInstance->instance->GetUnit((*i)->getUnitGuid()))
                        {
                            if (pTmp != pLichKing->getVictim() &&
                                pTmp->GetTypeId() == TYPEID_PLAYER &&
                                !pTmp->GetVehicle())
                            {
                                lPotentialTargets.push_back(pTmp->GetObjectGuid());
                            }
                        }
                    }
                }

                // select random target
                if (!lPotentialTargets.empty())
                {
                    std::list<ObjectGuid>::iterator i = lPotentialTargets.begin();
                    std::advance(i, urand(0, lPotentialTargets.size() - 1));

                    if (Unit *pTarget = m_pInstance->instance->GetUnit(*i))
                        return pTarget;
                }
            }
        }

        return NULL;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
        {
            m_creature->ForcedDespawn();
            return;
        }

        if (m_bIsWickedSpirit)
        {
            // Summon Spirit Bomb
            if (m_uiSummonSpiritBombTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SUMMON_SPIRIT_BOMB, CAST_TRIGGERED) == CAST_OK)
                    m_uiSummonSpiritBombTimer = 3000;
            }
            else
                m_uiSummonSpiritBombTimer -= uiDiff;
        }

        // start chasing timer
        if (!m_bIsReady)
        {
            if (m_uiReadyTimer < uiDiff)
            {
                Unit *pTarget = NULL;

                m_creature->SetInCombatWithZone();

                if (m_bIsWickedSpirit)
                    pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, SPELL_SPIRIT_BURST_AURA, SELECT_FLAG_PLAYER);
                else
                    pTarget = SelectTarget();

                if (pTarget)
                {
                    DoResetThreat();
                    m_creature->AddThreat(pTarget, 100000.0f);
                    m_bIsReady = true;
                    m_creature->StopMoving();
                    SetCombatMovement(true);
                    m_creature->GetMotionMaster()->Clear();

                    if (m_bIsWickedSpirit)
                        m_creature->SetWalk(true);

                    AttackStart(pTarget);
                }
            }
            else
                m_uiReadyTimer -= uiDiff;

        }
    }
};

CreatureAI* GetAI_mob_vile_spirit(Creature* pCreature)
{
    return new mob_vile_spiritAI(pCreature);
}


struct MANGOS_DLL_DECL mob_strangulate_vehicleAI : public base_icc_bossAI
{
    mob_strangulate_vehicleAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        SetCombatMovement(false);
        m_creature->SetWalk(true);
        m_bIsSetUp = false;
        m_bHasMoved = false;
        m_bHasTeleported = false;
        m_uiMoveTimer = 2000;
        m_uiTeleportTimer = 6000;
    }

    bool m_bIsSetUp;
    bool m_bHasMoved;
    bool m_bHasTeleported;
    uint32 m_uiMoveTimer;
    uint32 m_uiTeleportTimer;

    void Reset(){}
    void AttackStart(Unit *pWho){}

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            m_creature->ForcedDespawn();
        else
        {
            if (m_pInstance->GetData(TYPE_FROSTMOURNE_ROOM) == DONE)
            {
                if (Unit *pCreator = m_creature->GetCreator())
                {
                    float x, y, z;
                    m_creature->GetPosition(x, y, z);
                    pCreator->NearTeleportTo(x, y, z, m_creature->GetOrientation(), true);
                    // remove Harvest Soul aura
                    pCreator->RemoveAurasDueToSpell(72546);
                    pCreator->RemoveAurasDueToSpell(73655);

                    m_creature->ForcedDespawn();
                    return;
                }
            }
        }

        if (!m_bIsSetUp)
        {
            if (Unit *pCreator = m_creature->GetCreator())
            {
                pCreator->CastSpell(m_creature, SPELL_HARVEST_SOUL_VEHICLE, true);
                m_bIsSetUp = true;
            }
        }

        // move timer
        if (!m_bHasMoved)
        {
            if (m_uiMoveTimer < uiDiff)
            {
                if (m_pInstance)
                {
                    if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                    {
                        float x, y, z;
                        pLichKing->GetNearPoint(m_creature, x, y, z, m_creature->GetObjectBoundingRadius(), 8.0f, pLichKing->GetAngle(m_creature));
                        m_creature->GetMotionMaster()->MovePoint(POINT_TP_TO_FM, x, y, z, false);
                    }
                }
                m_bHasMoved = true;
            }
            else
                m_uiMoveTimer -= uiDiff;
        }

        // teleport timer
        if (!m_bHasTeleported)
        {
            if (m_uiTeleportTimer < uiDiff)
            {
                if (Unit *pCreator = m_creature->GetCreator())
                {
                    if (pCreator->isAlive())
                    {
                        pCreator->CastSpell(m_creature, SPELL_HARVEST_SOUL_CLONE, true);
                        pCreator->CastSpell(pCreator, SPELL_FROSTMOURNE_TP_VISUAL, true);
                        pCreator->CastSpell(pCreator, m_bIsHeroic ? SPELL_HARVEST_SOUL_TP_FM_H : SPELL_HARVEST_SOUL_TP_FM_N, true);
                    }
                    else
                    {
                        m_creature->ForcedDespawn();
                        return;
                    }
                }
                if (m_pInstance)
                {
                    if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                        pLichKing->InterruptSpell(CURRENT_CHANNELED_SPELL);
                }
                m_bHasTeleported = true;
            }
            else
                m_uiTeleportTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_mob_strangulate_vehicle(Creature* pCreature)
{
    return new mob_strangulate_vehicleAI(pCreature);
}

/**
 * Terenas Menethil in Frostmourne
 */
struct MANGOS_DLL_DECL npc_terenas_fmAI : public base_icc_bossAI
{
    npc_terenas_fmAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        m_uiSayPhase = 0;
        m_uiSayTimer = 6000;
        m_uiReleaseTimer = 46000;
        m_uiSummonWickedSpiritsTimer = 7000;
        m_uiSummonSpiritBombTimer = 7000;
        m_bHasReleasedPlayer = false;

        m_creature->ForcedDespawn(m_bIsHeroic ? 48000 : 68000);
    }

    uint32 m_uiSayTimer;
    uint32 m_uiSayPhase;
    uint32 m_uiReleaseTimer;
    uint32 m_uiSummonWickedSpiritsTimer;
    uint32 m_uiSummonSpiritBombTimer;
    bool m_bHasReleasedPlayer;

    void Reset(){}

    void Aggro(Unit *pWho)
    {
        DoCastSpellIfCan(m_creature, SPELL_LIGHTS_FAVOR, CAST_TRIGGERED);
        m_creature->SetHealthPercent(50.0f);
    }

    void EnterEvadeMode()
    {
        if (!m_bHasReleasedPlayer)
        {
            m_bHasReleasedPlayer = true;
            DoCastSpellIfCan(m_creature, SPELL_RESTORE_SOUL);
            m_uiSayPhase = 5;
            m_uiSayTimer = 2000;
        }
    }

    void JustDied(Unit *pKiller)
    {
        if (pKiller->GetEntry() == NPC_SPIRIT_WARDEN)
        {
            pKiller->CastSpell(pKiller, SPELL_DESTROY_SOUL, true);

            if (m_pInstance)
                m_pInstance->SetData(TYPE_FROSTMOURNE_ROOM, DONE);

            ((Creature*)pKiller)->ForcedDespawn();
        }
    }

    void DoSummonWickedSpirits(uint32 count)
    {
        for(uint32 i = 0; i < count; ++i)
        {
            float x, y, z;
            m_creature->GetPosition(x, y, z);

            z += 15.0f;
            x += frand(-25.0f, 25.0f);
            y += frand(-25.0f, 25.0f);

            m_creature->SummonCreature(NPC_WICKED_SPIRIT, x, y, z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 41000);
        }
    }

    void DamageTaken(Unit *pDealer, uint32 uiDamage)
    {
        if (m_bIsHeroic)
            uiDamage = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiSayTimer < uiDiff)
        {
            switch (m_uiSayPhase)
            {
                case 0:
                    if (!m_bIsHeroic)
                    {
                        if (Creature *pSpirit = GetClosestCreatureWithEntry(m_creature, NPC_SPIRIT_WARDEN, 20.0f))
                        {
                            AttackStart(pSpirit);
                            pSpirit->AI()->AttackStart(m_creature);
                            pSpirit->AddThreat(m_creature, 100000.0f);
                        }
                    }
                    else
                    {
                        DoSummonWickedSpirits(15);
                        m_uiSummonWickedSpiritsTimer = 15000;
                        // summon Spirit Bomb every 2 seconds
                        // aura doesnt work?;/
                        // DoCastSpellIfCan(m_creature, SPELL_SUMMON_SPIRIT_BOMBS_2, CAST_TRIGGERED);
                        DoCastSpellIfCan(m_creature, SPELL_RESTORE_SOUL_HEROIC);
                    }
                    ++m_uiSayPhase;
                    m_uiSayTimer = 1000;
                    break;
                case 1:
                    DoScriptText(SAY_FM_TERENAS_AID_1, m_creature);
                    ++m_uiSayPhase;
                    m_uiSayTimer = 8000;
                    break;
                case 2:
                    DoScriptText(SAY_FM_TERENAS_AID_2, m_creature);
                    ++m_uiSayPhase;
                    m_uiSayTimer = 12000;
                    break;
                case 3:
                    DoScriptText(SAY_FM_TERENAS_AID_3, m_creature);
                    ++m_uiSayPhase;
                    break;
                case 4:
                    // fight
                    break;
                case 5:
                    // release soul
                    if (m_pInstance)
                    {
                        m_pInstance->SetData(TYPE_FROSTMOURNE_ROOM, DONE);

                        if (Creature *pLichKing = m_pInstance->GetSingleCreatureFromStorage(NPC_LICH_KING))
                            DoScriptText(SAY_FM_PLAYER_ESCAPE, pLichKing);
                    }
                    ++m_uiSayPhase;
                    break;
                default:
                    break;
            }
        }
        else
            m_uiSayTimer -= uiDiff;

        if (!m_pInstance || m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        if (m_bIsHeroic)
        {
            // Release Souls (heroic)
            if (m_uiReleaseTimer < uiDiff)
            {
                if (m_pInstance)
                    m_pInstance->SetData(TYPE_FROSTMOURNE_ROOM, DONE);

                m_uiReleaseTimer = 46000;
            }
            else
                m_uiReleaseTimer -= uiDiff;

            // Summon Wicked Spirits
            if (m_uiSummonWickedSpiritsTimer < uiDiff)
            {
                DoSummonWickedSpirits(1);
                m_uiSummonWickedSpiritsTimer = 1000;
            }
            else
                m_uiSummonWickedSpiritsTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_terenas_fm(Creature* pCreature)
{
    return new npc_terenas_fmAI(pCreature);
}


/**
 * Spirit Warden
 */
struct MANGOS_DLL_DECL mob_spirit_wardenAI : public base_icc_bossAI
{
    mob_spirit_wardenAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        m_creature->ForcedDespawn(68000);
        Reset();
    }

    uint32 m_uiSoulRipTimer;
    uint32 m_uiHarvestedSoulTimer;

    void Reset()
    {
        DoCastSpellIfCan(m_creature, SPELL_DARK_HUNGER, CAST_TRIGGERED);
        m_uiSoulRipTimer = 12000;
        m_uiHarvestedSoulTimer = 61000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_pInstance || m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            m_creature->ForcedDespawn();

        // Soul Rip
        if (m_uiSoulRipTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SOUL_RIP) == CAST_OK)
                m_uiSoulRipTimer = 20000;
        }
        else
            m_uiSoulRipTimer -= uiDiff;

        // Harvest Soul 60sec aura wears off
        if (m_uiHarvestedSoulTimer < uiDiff)
        {
            if (m_pInstance)
                m_pInstance->SetData(TYPE_FROSTMOURNE_ROOM, DONE);

            m_uiHarvestedSoulTimer = 60000;
        }
        else
            m_uiHarvestedSoulTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_spirit_warden(Creature* pCreature)
{
    return new mob_spirit_wardenAI(pCreature);
}

/**
 * Spirit Bomb
 */
struct MANGOS_DLL_DECL mob_spirit_bombAI : public base_icc_bossAI
{
    mob_spirit_bombAI(Creature *pCreature) : base_icc_bossAI(pCreature)
    {
        m_creature->ForcedDespawn(41000);
        SetCombatMovement(false);
        m_creature->SetLevitate(true);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
        m_creature->SetInCombatWithZone();
        m_creature->GetMotionMaster()->Clear();

        float x, y, z;
        m_creature->GetPosition(x, y, z);

        m_creature->NearTeleportTo(x, y, z + 20.0f, 0.0f);
        m_creature->GetMotionMaster()->MovePoint(POINT_SPIRIT_BOMB, x, y, z, false);
        DoCastSpellIfCan(m_creature, SPELL_SPIRIT_BOMB_VISUAL, CAST_TRIGGERED);
    }

    void Reset(){}

    void MovementInform(uint32 uiMovementType, uint32 uiData)
    {
        if (uiMovementType != POINT_MOTION_TYPE)
            return;

        if (uiData == POINT_SPIRIT_BOMB)
        {
            DoCastSpellIfCan(m_creature, SPELL_EXPLOSION);
            m_creature->ForcedDespawn(1000);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_LICH_KING) != IN_PROGRESS)
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_mob_spirit_bomb(Creature* pCreature)
{
    return new mob_spirit_bombAI(pCreature);
}


void AddSC_boss_lich_king_icc()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_the_lich_king_icc";
    newscript->GetAI = &GetAI_boss_the_lich_king_icc;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_tirion_icc";
    newscript->GetAI = &GetAI_boss_tirion_icc;
    newscript->pGossipHello = &GossipHello_boss_tirion_icc;
    newscript->pGossipSelect = &GossipSelect_boss_tirion_icc;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ice_sphere_icc";
    newscript->GetAI = &GetAI_mob_ice_sphere_icc;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_shambling_horror";
    newscript->GetAI = &GetAI_mob_shambling_horror;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_drudge_ghoul";
    newscript->GetAI = &GetAI_mob_drudge_ghoul;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_shadow_trap";
    newscript->GetAI = &GetAI_mob_shadow_trap;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_raging_spirit";
    newscript->GetAI = &GetAI_mob_raging_spirit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_valkyr_shadowguard";
    newscript->GetAI = &GetAI_mob_valkyr_shadowguard;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_defiler_icc";
    newscript->GetAI = &GetAI_mob_defiler_icc;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vile_spirit";
    newscript->GetAI = &GetAI_mob_vile_spirit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_strangulate_vehicle";
    newscript->GetAI = &GetAI_mob_strangulate_vehicle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_terenas_fm";
    newscript->GetAI = &GetAI_npc_terenas_fm;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_spirit_warden";
    newscript->GetAI = &GetAI_mob_spirit_warden;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_spirit_bomb";
    newscript->GetAI = &GetAI_mob_spirit_bomb;
    newscript->RegisterSelf();
}
