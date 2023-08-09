/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "UpdateFields.h"
#include "ByteBuffer.h"

#if TRINITY_COMPILER == TRINITY_COMPILER_GNU
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#else
#pragma warning(push)
#pragma warning(disable: 4100)
#endif

namespace UF
{

    template<typename T>
    T& EnsureCapasity(std::vector<T> values, uint32 index)
    {
        values.emplace(values.begin() + index);

        return values[index];
    }


void ObjectData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags, IStatUpdate& update)
{
    data >> EntryID._value;
    data >> DynamicFlags._value;
    data >> Scale._value;
    //update.OnObjectDynamicFlags(0, DynamicFlags);
}

void ObjectData::ReadUpdate(ByteBuffer& data, IStatUpdate& update)
{
    uint32 mask = data.ReadBits(4);
    UpdateMask<4> changesMask(mask);
    data.ResetBits();

    if (changesMask[0])
    {
        if (changesMask[1])
        {
            data >> EntryID._value;
        }
        if (changesMask[2])
        {
            uint32 old = DynamicFlags;
            data >> DynamicFlags._value;
            update.OnObjectDynamicFlags(old, DynamicFlags);  
        }
        if (changesMask[3])
        {
            data >> Scale._value;
        }
    }
}

void ItemEnchantment::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> ID._value;
    data >> Duration._value;
    data >> Charges._value;
    data >> Unk254._value;
    data >> Unk254_2._value;
}

void ItemEnchantment::ReadUpdate(ByteBuffer& data)
{
    uint32 mask = data.ReadBits(5);
    UpdateMask<5> changesMask(mask);
    data.ResetBits();

    if (changesMask[0])
    {
        if (changesMask[1])
        {
            data >> ID._value;
        }
        if (changesMask[2])
        {
            data >> Duration._value;
        }
        if (changesMask[3])
        {
            data >> Charges._value;
        }
        if (changesMask[4])
        {
            data >> Unk254._value;
        }
        if (changesMask[5])
        {
            data >> Unk254_2._value;
        }
    }
}

void ItemMod::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> Value;
    data >> Type;
}

void ItemMod::ReadUpdate(ByteBuffer& data)
{
    data >> Value;
    data >> Type;
}


void ItemModList::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    uint32 size = data.ReadBits(6);
    data.ResetBits();
    for (std::size_t i = 0; i < size; ++i)
    {
        Values[i].ReadCreate(data, fieldVisibilityFlags);
    }
}

void ItemModList::ReadUpdate(ByteBuffer& data)
{
    uint32 mask = data.ReadBits(1);
    UpdateMask<1> changesMask(mask);
    uint32 size = 0;

    if (changesMask[0])
    {
        if (changesMask[0])
        {
            Values.ReadUpdateMask(data, 6);
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[0])
        {
            for (std::size_t i = 0; i < Values.UpdatedSize(); ++i)
            {
                if (Values.HasChanged(i))
                {
                    Values[i].ReadUpdate(data);
                }
            }
        }
    }
}

void ArtifactPower::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> ArtifactPowerID;
    data >> PurchasedRank;
    data >> CurrentRankWithBonus;
}

void ArtifactPower::ReadUpdate(ByteBuffer& data)
{
    data >> ArtifactPowerID;
    data >> PurchasedRank;
    data >> CurrentRankWithBonus;
}

void SocketedGem::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> ItemID._value;
    for (std::size_t i = 0; i < 16; ++i)
    {
        data >> BonusListIDs[i];
    }
    data >> Context._value;
}

void SocketedGem::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<32> changesMask;
    if (data.ReadBit())
        changesMask.Update(data.ReadBits(32));
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            data >> ItemID._value;
        }
        if (changesMask[2])
        {
            data >> Context._value;
        }
    }
    if (changesMask[3])
    {
        for (std::size_t i = 0; i < 16; ++i)
        {
            if (changesMask[4 + i])
            {
                data >> BonusListIDs[i];
            }
        }
    }
}

void ItemData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
 
    data >> Owner._value;
    data >> ContainedIn._value;
    data >> Creator._value;
    data >> GiftCreator._value;
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner))
    {
        data >> StackCount._value;
        data >> Expiration._value;
        for (std::size_t i = 0; i < 5; ++i)
        {
            data >> SpellCharges[i];
        }
    }
    data >> DynamicFlags._value;
    for (std::size_t i = 0; i < 13; ++i)
    {
        Enchantment[i].ReadCreate(data, fieldVisibilityFlags);
    }
    data >> PropertySeed._value;
    data >> RandomPropertiesID._value;
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner))
    {
        data >> Durability._value;
        data >> MaxDurability._value;
    }
    data >> CreatePlayedTime._value;
    data >> Context._value;
    data >> CreateTime._value;
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner))
    {
        data >> ArtifactXP._value;
        data >> ItemAppearanceModID._value;
    }
    uint32 apSize = data.read<uint32>();
    uint32 gemSize = data.read<uint32>();
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner))
    {
        data >> DynamicFlags2._value;
        data >> DEBUGItemLevel._value;
    }
    for (std::size_t i = 0; i < apSize; ++i)
    {
        ArtifactPowers[i].ReadCreate(data, fieldVisibilityFlags);
    }
    for (std::size_t i = 0; i < gemSize; ++i)
    {
        Gems[i].ReadCreate(data, fieldVisibilityFlags);
    }
    uint32 itemId = data.read<uint32>();
    uint32 size = data.read<uint32>();
    for (std::size_t i = 0; i < size; ++i)
    {
        uint32 id = data.read<uint32>();
        BonusListIDs._value.push_back(id);

    }
    Modifiers._value.ReadCreate(data, fieldVisibilityFlags);
}

void ItemData::ReadUpdate(ByteBuffer& data)
{
    uint32 maskdata[2] = { 0, 0 };
    UpdateMask<2> mask1(data.ReadBits(2));
    for (std::size_t i = 0; i < 2; ++i)
        if (mask1[i])
            maskdata[i] = data.ReadBits(32);
    UpdateMask<64> changesMask(maskdata, sizeof(maskdata)/sizeof(uint32));
    /*if (changesMask[0] && changesMask[1])
    {
        uint32 size = data.ReadBits(32);
        for (std::size_t i = 0; i < size; ++i)
        {
            data >> EnsureCapasity(BonusListIDs._value, i);
        }
    }*/
    data.ResetBits();

    if (changesMask[0])
    {
        if (changesMask[1])
            ArtifactPowers.ReadUpdateMask(data);
        if (changesMask[2])
             Gems.ReadUpdateMask(data);
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            for (std::size_t i = 0; i < ArtifactPowers.UpdatedSize(); ++i)
            {
                if (ArtifactPowers.HasChanged(i))
                {
                    ArtifactPowers[i].ReadUpdate(data);
                }
            }
        }
        if (changesMask[2])
        {
            for (std::size_t i = 0; i < Gems.UpdatedSize(); ++i)
            {
                if (Gems.HasChanged(i))
                {
                    Gems[i].ReadUpdate(data);
                }
            }
        }
        if (changesMask[3])
            data >> Owner._value;
        if (changesMask[4])
            data >> ContainedIn._value;
        if (changesMask[5])
            data >> Creator._value;
        if (changesMask[6])
            data >> GiftCreator._value;
        if (changesMask[7])
            data >> StackCount._value;//verified @3.4.2
        if (changesMask[8])
            data >> Expiration._value;
        if (changesMask[9])
            data >> DynamicFlags._value;
        if (changesMask[10])
            data >> PropertySeed._value;
        if (changesMask[11])
            data >> RandomPropertiesID._value;
        if (changesMask[12])
            data >> Durability._value;
        if (changesMask[13])
            data >> MaxDurability._value;
        if (changesMask[14])
            data >> CreatePlayedTime._value;
        if (changesMask[15])
            data >> Context._value;
        if (changesMask[16])
            data >> CreateTime._value;
        if (changesMask[17])
            data >> ArtifactXP._value;
        if (changesMask[18])
            data >> ItemAppearanceModID._value;
        if (changesMask[19])
            Modifiers._value.ReadUpdate(data);
        if (changesMask[20])
            data >> DynamicFlags2._value;
        if (changesMask[21])
            data >> DEBUGItemLevel._value;

    }
    if (changesMask[22])
    {
        for (std::size_t i = 0; i < 5; ++i)
        {
            if (changesMask[23 + i])
                data >> SpellCharges[i];
        }
    }
    if (changesMask[28])
    {
        for (std::size_t i = 0; i < 13; ++i)
        {
            if (changesMask[29 + i])
            {
                Enchantment[i].ReadUpdate(data);
            }
        }
    }
}

void ContainerData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags, IStatUpdate& update)
{
    for (std::size_t i = 0; i < 36; ++i)
    {
        data >> Slots[i];
    }
    data >> NumSlots._value;
}

void ContainerData::ReadUpdate(ByteBuffer& data, IStatUpdate& update)
{
    uint32 maskdata[2] = { 0, 0 };
    UpdateMask<2> mask1(data.ReadBits(2));
    for (std::size_t i = 0; i < 2; ++i)
        if (mask1[i])
            maskdata[i] = data.ReadBits(32);
    UpdateMask<64> changesMask(maskdata, sizeof(maskdata)/sizeof(uint32));

    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            data >> NumSlots._value;
        }
    }
    if (changesMask[2])
    {
        for (std::size_t i = 0; i < 36; ++i)
        {
            if (changesMask[3 + i])
            {
                uint32 old = 0;
                data >> Slots[i];
                update.OnContainerSlots(0, Slots[i]._low, i);
            }
        }
    }
}

void UnitChannel::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> SpellID;
    data >> SpellXSpellVisualID;
}

void UnitChannel::ReadUpdate(ByteBuffer& data)
{
    data >> SpellID;
    data >> SpellXSpellVisualID;
}

void VisibleItem::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> ItemID._value;
    data >> ItemAppearanceModID._value;
    data >> ItemVisual._value;
}

void VisibleItem::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<4> changesMask(data.ReadBits(4));

    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            data >> ItemID._value;
        }
        if (changesMask[2])
            data >> ItemAppearanceModID._value;

        if (changesMask[3])
        {
            data >> ItemVisual._value;
        }
    }
}

void PassiveSpellHistory::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> SpellID;
    data >> AuraSpellID;
}

void PassiveSpellHistory::ReadUpdate(ByteBuffer& data)
{
    data >> SpellID;
    data >> AuraSpellID;
}

void UnitData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags, IStatUpdate& update)
{
    data >> Health._value;
    update.OnUnitHealth(0, Health);
    data >> MaxHealth._value;
    data >> DisplayID._value;
    for (std::size_t i = 0; i < 2; ++i)
    {
        data >> NpcFlags[i];
    }
    data >> StateSpellVisualID._value;
    data >> StateAnimID._value;
    data >> StateAnimKitID._value;
    uint32 size = data.read<uint32>();
    for (std::size_t i = 0; i < size; ++i)
    {
        data >> EnsureCapasity(StateWorldEffectIDs._value, i);
    }
    data >> Charm._value;
    data >> Summon._value;
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner))
    {
        data >> Critter._value;
    }
    data >> CharmedBy._value;
    data >> SummonedBy._value;
    data >> CreatedBy._value;
    data >> DemonCreator._value;
    data >> LookAtControllerTarget._value;
    data >> Target._value;
    data >> BattlePetCompanionGUID._value;
    data >> BattlePetDBID._value;
    ChannelData._value.ReadCreate(data, fieldVisibilityFlags);
    data >> SummonedByHomeRealm._value;
    data >> Race._value;
    data >> ClassId._value;
    data >> PlayerClassId._value;
    data >> Sex._value;
    data >> DisplayPower._value;
    data >> OverrideDisplayPowerID._value;
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner | UpdateFieldFlag::UnitAll))
    {
        for (std::size_t i = 0; i < 10; ++i)//7 -> 10
        {
            data >> Unk340[i];
            data >> Unk340_2[i];
        }
    }
    for (std::size_t i = 0; i < 10; ++i)// 7 -> 10
    {
        data >> Power[i];
        data >> MaxPower[i];
        data >> PowerRegenFlatModifier[i];
        update.OnUnitPower(0, Power[i], i);
    }
    data >> Level._value;
    update.OnUnitLevel(0, Level);
    data >> EffectiveLevel._value;
    data >> ContentTuningID._value;
    data >> ScalingLevelMin._value;
    data >> ScalingLevelMax._value;
    data >> ScalingLevelDelta._value;
    data >> ScalingFactionGroup._value;
    data >> ScalingHealthItemLevelCurveID._value;
    data >> ScalingDamageItemLevelCurveID._value;
    data >> FactionTemplate._value;
    for (std::size_t i = 0; i < 3; ++i)
    {
        VirtualItems[i].ReadCreate(data, fieldVisibilityFlags);
    }
    data >> Flags._value;
    update.OnUnitFlags(0, Flags);
    data >> Flags2._value;
    data >> Flags3._value;
    data >> AuraState._value;
    for (std::size_t i = 0; i < 2; ++i)
    {
        data >> AttackRoundBaseTime[i];
    }
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner))
    {
        data >> RangedAttackRoundBaseTime._value;
    }
    data >> BoundingRadius._value;
    data >> CombatReach._value;
    data >> DisplayScale._value;
    data >> NativeDisplayID._value;
    data >> NativeXDisplayScale._value;
    data >> MountDisplayID._value;
    update.OnUnitDisplayId(0, MountDisplayID);
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner | UpdateFieldFlag::Empath))
    {
        data >> MinDamage._value;
        data >> MaxDamage._value;
        data >> MinOffHandDamage._value;
        data >> MaxOffHandDamage._value;
    }
    data >> StandState._value;
    data >> PetTalentPoints._value;
    data >> VisFlags._value;
    data >> AnimTier._value;
    data >> PetNumber._value;
    data >> PetNameTimestamp._value;
    data >> PetExperience._value;
    data >> PetNextLevelExperience._value;
    data >> ModCastingSpeed._value;
    data >> ModSpellHaste._value;
    data >> ModHaste._value;
    data >> ModRangedHaste._value;
    data >> ModHasteRegen._value;
    data >> ModTimeRate._value;
    data >> CreatedBySpell._value;
    data >> EmoteState._value;
    data >> TrainingPointsUsed._value;
    data >> TrainingPointsTotal._value;
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner))
    {
        for (std::size_t i = 0; i < 5; ++i)
        {
            data >> Stats[i];
            data >> StatPosBuff[i];
            data >> StatNegBuff[i];
        }
    }
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner | UpdateFieldFlag::Empath))
    {
        for (std::size_t i = 0; i < 7; ++i)
        {
            data >> Resistances[i];
        }
    }
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner))
    {
        for (std::size_t i = 0; i < 7; ++i)
        {
            data >> PowerCostModifier[i];
            data >> PowerCostMultiplier[i];
        }
    }
    for (std::size_t i = 0; i < 7; ++i)
    {
        data >> ResistanceBuffModsPositive[i];
        data >> ResistanceBuffModsNegative[i];
    }
    data >> BaseMana._value;
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner))
    {
        data >> BaseHealth._value;
    }
    data >> SheatheState._value;
    data >> PvpFlags._value;
    data >> PetFlags._value;
    data >> ShapeshiftForm._value;
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner))
    {
        data >> AttackPower._value;
        data >> AttackPowerModPos._value;
        data >> AttackPowerModNeg._value;
        data >> AttackPowerMultiplier._value;
        data >> RangedAttackPower._value;
        data >> RangedAttackPowerModPos._value;
        data >> RangedAttackPowerModNeg._value;
        data >> RangedAttackPowerMultiplier._value;
        data >> SetAttackSpeedAura._value;
        data >> Lifesteal._value;
        data >> MinRangedDamage._value;
        data >> MaxRangedDamage._value;
        data >> MaxHealthModifier._value;
    }
    data >> HoverHeight._value;
    data >> MinItemLevelCutoff._value;
    data >> MinItemLevel._value;
    data >> MaxItemLevel._value;
    data >> WildBattlePetLevel._value;
    data >> BattlePetCompanionNameTimestamp._value;
    data >> InteractSpellID._value;
    data >> ScaleDuration._value;
    data >> LooksLikeMountID._value;
    data >> LooksLikeCreatureID._value;
    data >> LookAtControllerID._value;
    uint32 unknown;
    data >> unknown;
    data >> GuildGUID._value;
    uint32 ps = data.read<uint32>();
    uint32 we = data.read<uint32>();
    uint32 co = data.read<uint32>();
    data >> SkinningOwnerGUID._value;
    data >> unknown;

    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::Owner))
    {
        data >> Unk340_3._value;
    }
    for (std::size_t i = 0; i < ps; ++i)
    {
        PassiveSpells[i].ReadCreate(data, fieldVisibilityFlags);
    }
    for (std::size_t i = 0; i < we; ++i)
    {
        data >> WorldEffects[i];
    }
    for (std::size_t i = 0; i < co; ++i)
    {
        data >> ChannelObjects[i];
    }
}

void UnitData::ReadUpdate(ByteBuffer& data, IStatUpdate &update)
{
    UpdateMask<8> mask(data.ReadBits(8));//changed 3.4.2 guess
    uint32 m[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    for (std::size_t i = 0; i < 8; ++i)
        if (mask[i])
            m[i] = data.ReadBits(32);

    UpdateMask<32 * 8> changesMask(m, sizeof(m)/sizeof(uint32));

    if (changesMask[0])
    {
        if (changesMask[1])
        {
            uint32 size = data.ReadBits(32);
            if (size)
            {
                data >> EnsureCapasity(StateWorldEffectIDs._value, size);
                for (std::size_t i = 0; i < size; ++i)
                {
                    data >> StateWorldEffectIDs._value[i];
                }
            }
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[2])
        {
                PassiveSpells.ReadUpdateMask(data);
        }
        if (changesMask[3])
        {
                WorldEffects.ReadUpdateMask(data);
        }
        if (changesMask[4])
        {
                ChannelObjects.ReadUpdateMask(data);
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[2])
        {
            for (uint32 i = 0; i < PassiveSpells.UpdatedSize(); i ++)
            {
                if (PassiveSpells.HasChanged(i))
                {
                    PassiveSpells[i].ReadUpdate(data);
                }
            }
        }
        if (changesMask[3])
        {
            for (std::size_t i = 0; i < WorldEffects.UpdatedSize(); ++i)
            {
                if (WorldEffects.HasChanged(i))
                {
                    data >> WorldEffects[i];
                }
            }
        }
        if (changesMask[4])
        {
            for (std::size_t i = 0; i < ChannelObjects.UpdatedSize(); ++i)
            {
                if (ChannelObjects.HasChanged(i))
                {
                    data >> ChannelObjects[i];
                }
            }
        }
        if (changesMask[5])
        {
            uint32 old = Health;
            data >> Health._value;
            update.OnUnitHealth(old, Health);
        }
        if (changesMask[6])
        {
            data >> MaxHealth._value;
        }
        if (changesMask[7])
        {
            data >> DisplayID._value;
            update.OnUnitDisplayId(0, DisplayID._value);
        }
        if (changesMask[8])
        {
            data >> StateSpellVisualID._value;
        }
        if (changesMask[9])
        {
            data >> StateAnimID._value;
        }
        if (changesMask[10])
        {
            data >> StateAnimKitID._value;
        }
        if (changesMask[11])
        {
            data >> Charm._value;
        }
        if (changesMask[12])
        {
            data >> Summon._value;
        }
        if (changesMask[13])
        {
            data >> Critter._value;
        }
        if (changesMask[14])
        {
            data >> CharmedBy._value;
        }
        if (changesMask[15])
        {
            data >> SummonedBy._value;
        }
        if (changesMask[16])
        {
            data >> CreatedBy._value;
        }
        if (changesMask[17])
        {
            data >> DemonCreator._value;
        }
        if (changesMask[18])
        {
            data >> LookAtControllerTarget._value;
        }
        if (changesMask[19])
        {
            data >> Target._value;
        }
        if (changesMask[20])
        {
            data >> BattlePetCompanionGUID._value;
        }
        if (changesMask[21])
        {
            data >> BattlePetDBID._value;
        }
        if (changesMask[22])
        {
            ChannelData._value.ReadUpdate(data);
        }
        if (changesMask[23])
        {
            data >> SummonedByHomeRealm._value;
        }
        if (changesMask[24])
        {
            data >> Race._value;
        }
        if (changesMask[25])
        {
            data >> ClassId._value;
        }
        if (changesMask[26])
        {
            data >> PlayerClassId._value;
        }
        if (changesMask[27])
        {
            data >> Sex._value;
        }
        if (changesMask[28])
        {
            data >> DisplayPower._value;
        }
        if (changesMask[29])
        {
            data >> OverrideDisplayPowerID._value;
        }
        if (changesMask[30])
        {
            uint32 old = Level;
            data >> Level._value;
            update.OnUnitLevel(old, Level);
        }
        if (changesMask[31])
        {
            data >> EffectiveLevel._value;
        }
    }
    if (changesMask[32])
    {
        if (changesMask[33])
        {
            data >> ContentTuningID._value;
        }
        if (changesMask[34])
        {
            data >> ScalingLevelMin._value;
        }
        if (changesMask[35])
        {
            data >> ScalingLevelMax._value;
        }
        if (changesMask[36])
        {
            data >> ScalingLevelDelta._value;
        }
        if (changesMask[37])
        {
            data >> ScalingFactionGroup._value;
        }
        if (changesMask[38])
        {
            data >> ScalingHealthItemLevelCurveID._value;
        }
        if (changesMask[39])
        {
            data >> ScalingDamageItemLevelCurveID._value;
        }
        if (changesMask[40])
        {
            data >> FactionTemplate._value;
        }
        if (changesMask[41])
        {
            uint32 old = Flags;
            data >> Flags._value;
            update.OnUnitFlags(old, Flags);
        }
        if (changesMask[42])
        {
            data >> Flags2._value;
        }
        if (changesMask[43])
        {
            data >> Flags3._value;
        }
        if (changesMask[44])
        {
            data >> AuraState._value;
        }
        if (changesMask[45])
        {
            data >> RangedAttackRoundBaseTime._value;
        }
        if (changesMask[46])
        {
            data >> BoundingRadius._value;
        }
        if (changesMask[47])
        {
            data >> CombatReach._value;
        }
        if (changesMask[48])
        {
            data >> DisplayScale._value;
            update.OnUnitDisplayId(0, 0);
        }
        if (changesMask[49])
        {
            data >> NativeDisplayID._value;
        }
        if (changesMask[50])
        {
            data >> NativeXDisplayScale._value;
        }
        if (changesMask[51])
        {
            data >> MountDisplayID._value;
            update.OnUnitDisplayId(0, 0);
        }
        if (changesMask[52])
        {
            data >> MinDamage._value;
        }
        if (changesMask[53])
        {
            data >> MaxDamage._value;
        }
        if (changesMask[54])
        {
            data >> MinOffHandDamage._value;
        }
        if (changesMask[55])
        {
            data >> MaxOffHandDamage._value;
        }
        if (changesMask[56])
        {
            data >> StandState._value;
        }
        if (changesMask[57])
        {
            data >> PetTalentPoints._value;
        }
        if (changesMask[58])
        {
            data >> VisFlags._value;
        }
        if (changesMask[59])
        {
            data >> AnimTier._value;
        }
        if (changesMask[60])
        {
            data >> PetNumber._value;
        }
        if (changesMask[61])
        {
            data >> PetNameTimestamp._value;
        }
        if (changesMask[62])
        {
            data >> PetExperience._value;
        }
        if (changesMask[63])
        {
            data >> PetNextLevelExperience._value;
        }    
    }
    if (changesMask[64])
    {

        if (changesMask[65])
        {
            data >> ModCastingSpeed._value;
        }
        if (changesMask[66])
        {
            data >> ModSpellHaste._value;
        }
        if (changesMask[67])
        {
            data >> ModHaste._value;
        }
        if (changesMask[68])
        {
            data >> ModRangedHaste._value;
        }
        if (changesMask[69])
        {
            data >> ModHasteRegen._value;
        }
        if (changesMask[70])
        {
            data >> ModTimeRate._value;
        }
        if (changesMask[71])
        {
            data >> CreatedBySpell._value;
        }
        if (changesMask[72])
        {
            data >> EmoteState._value;
        }
        if (changesMask[73])
        {
            data >> TrainingPointsUsed._value;
        }
        if (changesMask[74])
        {
            data >> TrainingPointsTotal._value;
        }
        if (changesMask[75])
        {
            data >> BaseMana._value;
        }
        if (changesMask[76])
        {
            data >> BaseHealth._value;
        }
        if (changesMask[77])
        {
            data >> SheatheState._value;
        }
        if (changesMask[78])
        {
            data >> PvpFlags._value;
        }
        if (changesMask[79])
        {
            data >> PetFlags._value;
        }
        if (changesMask[80])
        {
            data >> ShapeshiftForm._value;
        }
        if (changesMask[81])
        {
            data >> AttackPower._value;
        }
        if (changesMask[82])
        {
            data >> AttackPowerModPos._value;
        }
        if (changesMask[83])
        {
            data >> AttackPowerModNeg._value;
        }
        if (changesMask[84])
        {
            data >> AttackPowerMultiplier._value;
        }
        if (changesMask[85])
        {
            data >> RangedAttackPower._value;
        }
        if (changesMask[86])
        {
            data >> RangedAttackPowerModPos._value;
        }
        if (changesMask[87])
        {
            data >> RangedAttackPowerModNeg._value;
        }
        if (changesMask[88])
        {
            data >> RangedAttackPowerMultiplier._value;
        }
        if (changesMask[89])
        {
            data >> SetAttackSpeedAura._value;
        }
        if (changesMask[90])
        {
            data >> Lifesteal._value;
        }
        if (changesMask[91])
        {
            data >> MinRangedDamage._value;
        }
        if (changesMask[92])
        {
            data >> MaxRangedDamage._value;
        }
        if (changesMask[93])
        {
            data >> MaxHealthModifier._value;
        }
        if (changesMask[94])
        {
            data >> HoverHeight._value;
        }
        if (changesMask[95])
        {
            data >> MinItemLevelCutoff._value;
        }
    }
    if (changesMask[96])
    {

        if (changesMask[97])
        {
            data >> MinItemLevel._value;
        }
        if (changesMask[98])
        {
            data >> MaxItemLevel._value;
        }
        if (changesMask[99])
        {
            data >> WildBattlePetLevel._value;
        }
        if (changesMask[100])
        {
            data >> BattlePetCompanionNameTimestamp._value;
        }
        if (changesMask[101])
        {
            data >> InteractSpellID._value;
        }
        if (changesMask[102])
        {
            data >> ScaleDuration._value;
        }
        if (changesMask[103])
        {
            data >> LooksLikeMountID._value;
        }
        if (changesMask[104])
        {
            data >> LooksLikeCreatureID._value;
        }
        if (changesMask[105])
        {
            data >> LookAtControllerID._value;
        }
        /////////////////////////////////
        if (changesMask[106])//guess, order maybe different below
        {
            uint32 unk;
            data >> unk;
        }
        if (changesMask[107])
        {
            data >> GuildGUID._value;
        }
        if (changesMask[108])
        {
            data >> SkinningOwnerGUID._value;
        }
        if (changesMask[109])
        {
            uint32 unk;
            data >> unk;
        }
        if (changesMask[110])
        {
            data >> Unk340_3._value;//verfied @3.4.2
        }
     }
    if (changesMask[111])
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            if (changesMask[112 + i])
            {
                data >> NpcFlags[i];
            }
        }
    }
    if (changesMask[114])
    {
        for (std::size_t i = 0; i < 10; ++i)
        {
            if (changesMask[115 + i])//verified @3.4.2
            {
                data >> Unk340[i];
            }
            if (changesMask[125 + i])
            {
                data >> Unk340_2[i];
            }
            if (changesMask[135 + i])
            {
                uint32 old = Power[i]; //position verifyed @3.4.2
                data >> Power[i];
                update.OnUnitPower(old, Power[i], i);
            }
            if (changesMask[145 + i])
            {
                data >> MaxPower[i];
            }
            if (changesMask[155 + i])
            {
                data >> PowerRegenFlatModifier[i];
            }
        }
    }
    if (changesMask[165])
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            if (changesMask[166 + i])
            {
                VirtualItems[i].ReadUpdate(data);
            }
        }
    }
    if (changesMask[169])
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            if (changesMask[170 + i])
            {
                data >> AttackRoundBaseTime[i];
            }
        }
    }
    if (changesMask[172])
    {
        for (std::size_t i = 0; i < 5; ++i)
        {
            if (changesMask[173 + i])//verified @3.4.2
            {
                data >> Stats[i];
            }
            if (changesMask[178 + i])
            {
                data >> StatPosBuff[i];
            }
            if (changesMask[183 + i])
            {
                data >> StatNegBuff[i];
            }
        }
    }
    if (changesMask[188])
    {
        for (std::size_t i = 0; i < 7; ++i)
        {
            if (changesMask[189 + i])//verified @3.4.2
            {
                data >> Resistances[i];
            }
            if (changesMask[196 + i])
            {
                data >> PowerCostModifier[i];
            }
            if (changesMask[203 + i])
            {
                data >> PowerCostMultiplier[i];
            }
        }
    }
    if (changesMask[210])
    {
        for (std::size_t i = 0; i < 7; ++i)
        {
            if (changesMask[211 + i])
            {
                data >> ResistanceBuffModsPositive[i];
            }
            if (changesMask[218 + i])
            {
                data >> ResistanceBuffModsNegative[i];
            }
        }
    }
}

void ChrCustomizationChoice::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> ChrCustomizationOptionID;
    data >> ChrCustomizationChoiceID;
}

void ChrCustomizationChoice::ReadUpdate(ByteBuffer& data)
{
    data >> ChrCustomizationOptionID;
    data >> ChrCustomizationChoiceID;
}

void QuestLog::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags, uint32 index, IStatUpdate& update)
{
    uint32 old = QuestID;
    data >> QuestID._value;
    bool same = (old == QuestID);
    if (!same)
        update.OnPlayerQuestLogId(old, QuestID, index);

    data >> StateFlags._value;
    data >> EndTime._value;
    data >> AcceptTime._value;
    for (std::size_t i = 0; i < 24; ++i)
    {
        uint16 progress = same ? ObjectiveProgress[i] : 0;
        data >> ObjectiveProgress[i];
        if (ObjectiveProgress[i])
            update.OnPlayerQuestLogCount(progress, ObjectiveProgress[i], index, i);
    }
}

void QuestLog::ReadUpdate(ByteBuffer& data, uint32 index, IStatUpdate& update)
{
    uint32 mask = 0;
    if (data.ReadBit())
        mask = data.ReadBits(32);
    UpdateMask<32> changesMask(mask);
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            uint32 old = QuestID;
            data >> QuestID._value;
            if (old != QuestID)
                update.OnPlayerQuestLogId(old, QuestID, index);
            if (QuestID == 0)
                Reset();
        }
        if (changesMask[2])
        {
            data >> StateFlags._value;
        }
        if (changesMask[3])
        {
            data >> EndTime._value;
        }
        if (changesMask[4])
        {
            data >> AcceptTime._value;
        }
    }
    if (changesMask[5])
    {
        for (std::size_t i = 0; i < 24; ++i)
        {
            if (changesMask[6 + i])
            {
                uint32 old = ObjectiveProgress[i];
                data >> ObjectiveProgress[i];
                update.OnPlayerQuestLogCount(old, ObjectiveProgress[i], index, i);
            }
        }
    }
}

void ArenaCooldown::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> SpellID._value;
    data >> Charges._value;
    data >> Unk254._value;
    data >> Flags._value;
    data >> StartTime._value;
    data >> EndTime._value;
    data >> NextChargeTime._value;
    data >> MaxCharges._value;
}

void ArenaCooldown::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<9> changesMask(data.ReadBits(9));
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            data >> SpellID._value;
        }
        if (changesMask[2])
        {
            data >> Charges._value;
        }
        if (changesMask[3])
        {
            data >> Unk254._value;
        }
        if (changesMask[4])
        {
            data >> Flags._value;
        }
        if (changesMask[5])
        {
            data >> StartTime._value;
        }
        if (changesMask[6])
        {
            data >> EndTime._value;
        }
        if (changesMask[7])
        {
            data >> NextChargeTime._value;
        }
        if (changesMask[8])
        {
            data >> MaxCharges._value;
        }
    }
}

void PlayerData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags, IStatUpdate& update)
{
    data >> DuelArbiter._value;
    data >> WowAccount._value;
    data >> LootTargetGUID._value;
    data >> PlayerFlags._value;
    data >> PlayerFlagsEx._value;
    data >> GuildRankID._value;
    data >> GuildDeleteDate._value;
    data >> GuildLevel._value;
    uint32 csize = data.read<uint32>();
    data >> PartyType._value;
    data >> NativeSex._value;
    data >> Inebriation._value;
    data >> PvpTitle._value;
    data >> ArenaFaction._value;
    data >> PvpRank._value;
    data >> Unk254._value;
    data >> DuelTeam._value;
    data >> GuildTimeStamp._value;
    if (fieldVisibilityFlags.HasFlag(UpdateFieldFlag::PartyMember))
    {
        for (std::size_t i = 0; i < 25; ++i)
        {
            QuestLog[i].ReadCreate(data, fieldVisibilityFlags, i, update);
        }
    }
    for (std::size_t i = 0; i < 19; ++i)
    {
        VisibleItems[i].ReadCreate(data, fieldVisibilityFlags);
    }
    data >> PlayerTitle._value;
    data >> FakeInebriation._value;
    data >> VirtualPlayerRealm._value;
    data >> CurrentSpecID._value;
    data >> TaxiMountAnimKitID._value;
    for (std::size_t i = 0; i < 6; ++i)
    {
        data >> AvgItemLevel[i];
    }
    data >> CurrentBattlePetBreedQuality._value;
    data >> HonorLevel._value;
    uint64 LogoutTime;
    data >> LogoutTime;
    uint32 acsize = data.read<uint32>();//position verified @3.4.2
    uint32 Field_B0, Field_B4;//guess
    data >> Field_B0 >> Field_B0;//guess

    for (std::size_t i = 0; i < 19; ++i)//guess @3.4.2
    {
        uint32 Field_3120;
        data >> Field_3120;
    }

    for (std::size_t i = 0; i < csize; ++i)
    {
        Customizations[i].ReadCreate(data, fieldVisibilityFlags);
    }
    for (std::size_t i = 0; i < acsize; ++i)
    {
        ArenaCooldowns[i].ReadCreate(data, fieldVisibilityFlags);
    }
}

void PlayerData::ReadUpdate(ByteBuffer& data, IStatUpdate& update)
{
    UpdateMask<4> mask(data.ReadBits(4));
    uint32 m[4] = { 0, 0, 0, 0 };
    for (std::size_t i = 0; i < 4; ++i)
        if (mask[i])
            m[i] = data.ReadBits(32);

    UpdateMask<32 * 4> changesMask(m, sizeof(m)/sizeof(uint32));
    bool noQuestLogChangesMask = data.ReadBit();

    if (changesMask[0])
    {
        if (changesMask[1])
        {
            Customizations.ReadUpdateMask(data);
        }
        if (changesMask[2])
        {
            ArenaCooldowns.ReadUpdateMask(data);
        }
        if (changesMask[3])
        {
            assert(false);
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            for (std::size_t i = 0; i < Customizations.UpdatedSize(); ++i)
            {
                if (Customizations.HasChanged(i))
                {
                    Customizations[i].ReadUpdate(data);
                }
            }
        }
        if (changesMask[2])
        {
            for (std::size_t i = 0; i < ArenaCooldowns.UpdatedSize(); ++i)
            {
                if (ArenaCooldowns.HasChanged(i))
                {
                    ArenaCooldowns[i].ReadUpdate(data);
                }
            }
        }
        if (changesMask[3])
        {
            assert(false);
        }

        if (changesMask[4])
        {
            data >> DuelArbiter._value;
        }
        if (changesMask[5])
        {
            data >> WowAccount._value;
        }
        if (changesMask[6])
        {
            data >> LootTargetGUID._value;
        }
        if (changesMask[7])
        {
            data >> PlayerFlags._value;
        }
        if (changesMask[8])
        {
            data >> PlayerFlagsEx._value;
        }
        if (changesMask[9])
        {
            data >> GuildRankID._value;
        }
        if (changesMask[10])
        {
            data >> GuildDeleteDate._value;
        }
        if (changesMask[11])
        {
            data >> GuildLevel._value;
        }
        if (changesMask[12])
        {
            data >> PartyType._value;
        }
        if (changesMask[13])
        {
            data >> NativeSex._value;
        }
        if (changesMask[14])
        {
            data >> Inebriation._value;
        }
        if (changesMask[15])
        {
            data >> PvpTitle._value;
        }
        if (changesMask[16])
        {
            data >> ArenaFaction._value;
        }
        if (changesMask[17])
        {
            data >> PvpRank._value;
        }
        if (changesMask[18])
        {
            data >> Unk254._value;
        }
        if (changesMask[19])
        {
            data >> DuelTeam._value;
        }
        if (changesMask[20])
        {
            data >> GuildTimeStamp._value;
        }
        if (changesMask[21])
        {
            data >> PlayerTitle._value;
        }
        if (changesMask[22])
        {
            data >> FakeInebriation._value;
        }
        if (changesMask[23])
        {
            data >> VirtualPlayerRealm._value;
        }
        if (changesMask[24])
        {
            data >> CurrentSpecID._value;
        }
        if (changesMask[25])
        {
            data >> TaxiMountAnimKitID._value;
        }
        if (changesMask[26])
        {
            data >> CurrentBattlePetBreedQuality._value;
        }
        if (changesMask[27])
        {
            data >> HonorLevel._value;
        }
        /////////////////////
        if (changesMask[28])
        {
            uint64 LogoutTime;
            data >> LogoutTime;
        }
        if (changesMask[29])
        {
            uint32 Field_B0;
            data >> Field_B0;
        }
        /*if (changesMask[29])
        {
            uint32 Field_B4;
            data >> Field_B4;
        }*/

    }
    if (changesMask[30])
    {
        EnumFlag<UF::UpdateFieldFlag> f((UF::UpdateFieldFlag)0);
        for (std::size_t i = 0; i < 25; ++i)
        {
            if (changesMask[31 + i])
            {
                if (noQuestLogChangesMask)
                    QuestLog[i].ReadCreate(data, f, i, update);//verified @3.4.2
                else
                    QuestLog[i].ReadUpdate(data, i, update);
            }
        }
    }
    if (changesMask[56])
    {
        for (std::size_t i = 0; i < 19; ++i)
        {
            if (changesMask[57 + i])
            {
                VisibleItems[i].ReadUpdate(data);
            }
        }
    }
    if (changesMask[76])
    {
        for (std::size_t i = 0; i < 6; ++i)
        {
            if (changesMask[77 + i])
            {
                data >> AvgItemLevel[i];
            }
        }
    }
    if (changesMask[83])//guess @3.4.2  
    {
        for (std::size_t i = 0; i < 19; ++i)
        {
            if (changesMask[84 + i])
            {
                uint32 Field_3120;
                data >> Field_3120;
            }
        }

    }
}

void SkillInfo::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags, IStatUpdate& update)
{
    for (std::size_t i = 0; i < 256; ++i)
    {
        data >> SkillLineID[i];
        data >> SkillStep[i];
        data >> SkillRank[i];
        data >> SkillStartingRank[i];
        data >> SkillMaxRank[i];
        data >> SkillTempBonus[i];
        data >> SkillPermBonus[i];
        if (SkillLineID[i])
            update.OnActivePlayerSkillLineId(0, SkillLineID[i], i);
    }
}

void SkillInfo::ReadUpdate(ByteBuffer& data, IStatUpdate& update)
{
    uint32 m[2] = { data.read<uint32>(), data.ReadBits(25) };
    UpdateMask<57> msk(m, sizeof(m)/sizeof(uint32));
    uint32 v[57] = { 0 };
    memset(v, 0, sizeof(v));
    for (std::size_t i = 0; i < 57; ++i)
        if (msk[i])
            v[i] = data.ReadBits(32);
    UpdateMask<1793> changesMask(v, sizeof(v)/sizeof(uint32));

    data.ResetBits();
    if (changesMask[0])
    {
        for (std::size_t i = 0; i < 256; ++i)
        {
            if (changesMask[1 + i])
            {
                uint32 old = SkillLineID[i];
                data >> SkillLineID[i];
                update.OnActivePlayerSkillLineId(old, SkillLineID[i], i);
            }
            if (changesMask[257 + i])
            {
                data >> SkillStep[i];
            }
            if (changesMask[513 + i])
            {
                data >> SkillRank[i];
            }
            if (changesMask[769 + i])
            {
                data >> SkillStartingRank[i];
            }
            if (changesMask[1025 + i])
            {
                data >> SkillMaxRank[i];
            }
            if (changesMask[1281 + i])
            {
                data >> SkillTempBonus[i];
            }
            if (changesMask[1537 + i])
            {
                data >> SkillPermBonus[i];
            }
        }
    }
}

void RestInfo::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> Threshold._value;
    data >> StateID._value;
}

void RestInfo::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<3> changesMask(data.ReadBits(3));
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            data >> Threshold._value;
        }
        if (changesMask[2])
        {
            data >> StateID._value;
        }
    }
}

void PVPInfo::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> Bracket._value;
    data >> PvpRatingID._value;
    data >> WeeklyPlayed._value;
    data >> WeeklyWon._value;
    data >> SeasonPlayed._value;
    data >> SeasonWon._value;
    data >> Rating._value;
    data >> WeeklyBestRating._value;
    data >> SeasonBestRating._value;
    data >> PvpTierID._value;
    data >> WeeklyBestWinPvpTierID._value;
    data >> Field_28._value;
    data >> Field_2C._value;
    data >> WeeklyRoundsPlayed._value;
    data >> WeeklyRoundsWon._value;
    data >> SeasonRoundsPlayed._value;
    data >> SeasonRoundsWon._value;
    Disqualified._value = data.ReadBit();
    data.ResetBits();
}

void PVPInfo::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<13> changesMask(data.ReadBits(19));
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            Disqualified._value = data.ReadBit();
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[2])
        {
            data >> Bracket._value;
        }
        if (changesMask[3])
        {
            data >> PvpRatingID._value;
        }
        if (changesMask[4])
        {
            data >> WeeklyPlayed._value;
        }
        if (changesMask[5])
        {
            data >> WeeklyWon._value;
        }
        if (changesMask[6])
        {
            data >> SeasonPlayed._value;
        }
        if (changesMask[7])
        {
            data >> SeasonWon._value;
        }
        if (changesMask[8])
        {
            data >> Rating._value;
        }
        if (changesMask[9])
        {
            data >> WeeklyBestRating._value;
        }
        if (changesMask[10])
        {
            data >> SeasonBestRating._value;
        }
        if (changesMask[11])
        {
            data >> PvpTierID._value;
        }
        if (changesMask[12])
        {
            data >> WeeklyBestWinPvpTierID._value;
        }
        if (changesMask[13])
        {
            data >> Field_28._value;
        }
        if (changesMask[14])
        {
            data >> Field_2C._value;
        }
        if (changesMask[15])
        {
            data >> WeeklyRoundsPlayed._value;
        }
        if (changesMask[16])
        {
            data >> WeeklyRoundsWon._value;
        }
        if (changesMask[17])
        {
            data >> SeasonRoundsPlayed._value;
        }
        if (changesMask[18])
        {
            data >> SeasonRoundsWon._value;
        }
    }
    data.ResetBits();
}


void CharacterRestriction::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> Field_0;
    data >> Field_4;
    data >> Field_8;
    Type = data.ReadBits(5);
    data.ResetBits();
}

void CharacterRestriction::ReadUpdate(ByteBuffer& data)
{
    data >> Field_0;
    data >> Field_4;
    data >> Field_8;
    Type = data.ReadBits(5);
    data.ResetBits();
}

void SpellPctModByLabel::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> ModIndex;
    data >> ModifierValue;
    data >> LabelID;
}

void SpellPctModByLabel::ReadUpdate(ByteBuffer& data)
{
    data >> ModIndex;
    data >> ModifierValue;
    data >> LabelID;
}

void SpellFlatModByLabel::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> ModIndex;
    data >> ModifierValue;
    data >> LabelID;
}

void SpellFlatModByLabel::ReadUpdate(ByteBuffer& data)
{
    data >> ModIndex;
    data >> ModifierValue;
    data >> LabelID;
}

void Research::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> ResearchProjectID;
}

void Research::ReadUpdate(ByteBuffer& data)
{
    data >> ResearchProjectID;
}

void GlyphInfo::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> GlyphSlot._value;
    data >> Glyph._value;
}

void GlyphInfo::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<3> changesMask(data.ReadBits(3));
    data.ResetBits();

    if (changesMask[0])
    {
        if (changesMask[1])
        {
            data >> GlyphSlot._value;
        }
        if (changesMask[2])
        {
            data >> Glyph._value;
        }
    }
}


void ActivePlayerData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags, IStatUpdate& update)
{
    uint32 pos = data.rpos();
    for (std::size_t i = 0; i < 129 + 12; ++i)
    {
        data >> InvSlots[i];
    }
    data >> FarsightObject._value;
    data >> ComboTarget._value;
    uint32 titlesize = data.read<uint32>();
    data >> Coinage._value;
    update.OnActivePlayerCoinage(0, Coinage);
    data >> XP._value;
    data >> NextLevelXP._value;
    data >> TrialXP._value;
    Skill._value.ReadCreate(data, fieldVisibilityFlags, update);
    data >> CharacterPoints._value;
    data >> MaxTalentTiers._value;
    data >> TrackCreatureMask._value;
    for (std::size_t i = 0; i < 2; ++i)
    {
        data >> TrackResourceMask[i];
    }
    data >> MainhandExpertise._value;
    data >> OffhandExpertise._value;
    data >> RangedExpertise._value;
    data >> CombatRatingExpertise._value;
    data >> BlockPercentage._value;
    data >> DodgePercentage._value;
    data >> DodgePercentageFromAttribute._value;
    data >> ParryPercentage._value;
    data >> ParryPercentageFromAttribute._value;
    data >> CritPercentage._value;
    data >> RangedCritPercentage._value;
    data >> OffhandCritPercentage._value;
    for (std::size_t i = 0; i < 7; ++i)
    {
        data >> SpellCritPercentage[i];
        data >> ModDamageDonePos[i];
        data >> ModDamageDoneNeg[i];
        data >> ModDamageDonePercent[i];
    }
    data >> ShieldBlock._value;
    data >> Mastery._value;
    data >> Speed._value;
    data >> Avoidance._value;
    data >> Sturdiness._value;
    data >> Unk340_3._value;
    data >> Versatility._value;
    data >> VersatilityBonus._value;
    data >> PvpPowerDamage._value;
    data >> PvpPowerHealing._value;
    for (std::size_t i = 0; i < 240; ++i)
    {
        data >> ExploredZones[i];
    }
    for (std::size_t i = 0; i < 2; ++i)
    {
        RestInfo[i].ReadCreate(data, fieldVisibilityFlags);
    }
    data >> ModHealingDonePos._value;
    data >> ModHealingPercent._value;
    data >> ModHealingDonePercent._value;
    data >> ModPeriodicHealingDonePercent._value;
    for (std::size_t i = 0; i < 3; ++i)
    {
        data >> WeaponDmgMultipliers[i];
        data >> WeaponAtkSpeedMultipliers[i];
    }
    data >> ModSpellPowerPercent._value;
    data >> ModResiliencePercent._value;
    data >> OverrideSpellPowerByAPPercent._value;
    data >> OverrideAPBySpellPowerPercent._value;
    data >> ModTargetResistance._value;
    data >> ModTargetPhysicalResistance._value;
    data >> LocalFlags._value;
    data >> GrantableLevels._value;
    data >> MultiActionBars._value;
    data >> LifetimeMaxRank._value;
    data >> NumRespecs._value;
    data >> AmmoID._value;
    data >> PvpMedals._value;
    for (std::size_t i = 0; i < 12; ++i)
    {
        data >> BuybackPrice[i];
        data >> BuybackTimestamp[i];
    }
    data >> TodayHonorableKills._value;
    data >> TodayDishonorableKills._value;
    data >> YesterdayHonorableKills._value;
    data >> YesterdayDishonorableKills._value;
    data >> LastWeekHonorableKills._value;
    data >> LastWeekDishonorableKills._value;
    data >> ThisWeekHonorableKills._value;
    data >> ThisWeekDishonorableKills._value;
    data >> ThisWeekContribution._value;
    data >> LifetimeHonorableKills._value;
    data >> LifetimeDishonorableKills._value;
    data >> YesterdayContribution._value;
    data >> LastWeekContribution._value;
    data >> LastWeekRank._value;
    data >> WatchedFactionIndex._value;
    for (std::size_t i = 0; i < 32; ++i)
    {
        data >> CombatRatings[i];
    }
    data >> MaxLevel._value;//60, 70, 80 ....
    data >> ScalingPlayerLevelDelta._value;
    data >> MaxCreatureScalingLevel._value;
    for (std::size_t i = 0; i < 4; ++i)
    {
        data >> NoReagentCostMask[i];
    }
    data >> PetSpellPower._value;
    for (std::size_t i = 0; i < 2; ++i)
    {
        data >> ProfessionSkillLine[i];//pos verified @3.4.2
    }
    data >> UiHitModifier._value;
    data >> UiSpellHitModifier._value;
    data >> HomeRealmTimeOffset._value;
    data >> ModPetHaste._value;
    //JailersTowerLevelMax, JailersTowerLevel
    data >> LocalRegenFlags._value;
    data >> AuraVision._value;
    data >> NumBackpackSlots._value;
    data >> OverrideSpellsID._value;
    data >> LfgBonusFactionID._value;
    data >> LootSpecID._value;
    data >> OverrideZonePVPType._value;
    for (std::size_t i = 0; i < 4; ++i)
    {
        data >> BagSlotFlags[i];
    }
    for (std::size_t i = 0; i < 7; ++i)
    {
        data >> BankBagSlotFlags[i];
    }
    for (std::size_t i = 0; i < 875; ++i)
    {
        data >> QuestCompleted[i];
    }
    data >> Honor._value;
    data >> HonorNextLevel._value;//5500
    data >> PvpTierMaxFromWins._value;
    data >> PvpLastWeeksTierMaxFromWins._value;
    data >> NumBankSlots._value;
    uint32 rsSize = data.read<uint32>();
    uint32 rspsize = data.read<uint32>();

    uint32 dqcsize = data.read<uint32>();
    uint32 aqlxqsize = data.read<uint32>();

    uint32 Unk254size = data.read<uint32>();//not exist in 10.1.5

    uint32 hsize = data.read<uint32>();
    uint32 hfsize = data.read<uint32>();

    uint32 tsize = data.read<uint32>();
    //ToyFlags
    uint32 tmsize = data.read<uint32>();
    uint32 ctmsize = data.read<uint32>();

    uint32 srssize = data.read<uint32>();
    //RuneforgePowers
    //TransmogIllusions
    uint32 crsize = data.read<uint32>();
    uint32 spmlsize = data.read<uint32>();
    uint32 sfmlsize = data.read<uint32>();
    for (std::size_t i = 0; i < 1; ++i)
    {
        uint32 size = data.read<uint32>();
        for (std::size_t j = 0; j < size; ++j)
        {
            Research[i][j].ReadCreate(data, fieldVisibilityFlags);
        }
    }
    uint32 UiChromieTimeExpansionID, unk2, unk3 = 0;
    data >> UiChromieTimeExpansionID;
    data >> TransportServerTime._value;//position verifyed
    data >> unk2 >> unk3;
    for (std::size_t i = 0; i < 6; ++i)//position verifyed  0x15 00 00 00 00 00 00 00 0x16 00 00 00 00 00 00 00 0x17 00 ....
    {
        GlyphInfos[i].ReadCreate(data, fieldVisibilityFlags);
    }
    data >> GlyphsEnabled._value;
    data >> Unk340._value;

    for (std::size_t i = 0; i < titlesize; ++i)
    {
        data >> KnownTitles[i];
    }
    for (std::size_t i = 0; i < rsSize; ++i)
    {
        data >> ResearchSites[i];
    }
    for (std::size_t i = 0; i < rspsize; ++i)
    {
        data >> ResearchSiteProgress[i];
    }
    for (std::size_t i = 0; i < dqcsize; ++i)
    {
        data >> DailyQuestsCompleted[i];
    }
    for (std::size_t i = 0; i < aqlxqsize; ++i)
    {
        data >> AvailableQuestLineXQuestIDs[i];
    }
    for (std::size_t i = 0; i < Unk254size; ++i)
    {
        data >> Unk254[i];
    }
    for (std::size_t i = 0; i < hsize; ++i)
    {
        data >> Heirlooms[i];
    }
    for (std::size_t i = 0; i < hfsize; ++i)
    {
        data >> HeirloomFlags[i];
    }
    for (std::size_t i = 0; i < tsize; ++i)
    {
        data >> Toys[i];
    }
    for (std::size_t i = 0; i < tmsize; ++i)
    {
        data >> Transmog[i];
    }
    for (std::size_t i = 0; i < ctmsize; ++i)
    {
        data >> ConditionalTransmog[i];
    }
    for (std::size_t i = 0; i < srssize; ++i)
    {
        data >> SelfResSpells[i];
    }
    for (std::size_t i = 0; i < spmlsize; ++i)
    {
        SpellPctModByLabel[i].ReadCreate(data, fieldVisibilityFlags);
    }
    for (std::size_t i = 0; i < sfmlsize; ++i)
    {
        SpellFlatModByLabel[i].ReadCreate(data, fieldVisibilityFlags);
    }
    for (std::size_t i = 0; i < 7; ++i)
    {
        PvpInfo[i].ReadCreate(data, fieldVisibilityFlags);
    }
    (bool&)InsertItemsLeftToRight = data.ReadBit();
    data.ResetBits();
    for (std::size_t i = 0; i < crsize; ++i)
    {
        CharacterRestrictions[i].ReadCreate(data, fieldVisibilityFlags);
    }

    data.rpos(data.rpos() + 49);
}

void ActivePlayerData::ReadUpdate(ByteBuffer& data, IStatUpdate& update)
{
    uint32 m[2] = { data.read<uint32>(), data.ReadBits(16) };//changed 3.4.2 guess
    UpdateMask<48> msk(m, sizeof(m)/sizeof(uint32));
    uint32 v[48];
    memset(v, 0, sizeof(v));
    for (std::size_t i = 0; i < 48; ++i)
        if (msk[i])
            v[i] = data.ReadBits(32);
    UpdateMask<32 * 48> changesMask(v, sizeof(v)/sizeof(uint32));

    if (changesMask[0])
    {
        if (changesMask[1])
        {
            (bool&)InsertItemsLeftToRight = data.ReadBit();
        }
        if (changesMask[2])
        {
            KnownTitles.ReadUpdateMask(data);
        }
        if (changesMask[3])
        {
            ResearchSites.ReadUpdateMask(data);
        }
        if (changesMask[4])
        {
            ResearchSiteProgress.ReadUpdateMask(data);
        }
        if (changesMask[5])
        {
            DailyQuestsCompleted.ReadUpdateMask(data);
        }
        if (changesMask[6])
        {
            AvailableQuestLineXQuestIDs.ReadUpdateMask(data);
        }
        if (changesMask[7])
        {
            Unk254.ReadUpdateMask(data);
        }
        if (changesMask[8])
        {
            Heirlooms.ReadUpdateMask(data);
        }
        if (changesMask[9])
        {
            HeirloomFlags.ReadUpdateMask(data);
        }
        if (changesMask[10])
        {
            Toys.ReadUpdateMask(data);
        }
        if (changesMask[11])
        {
            Transmog.ReadUpdateMask(data);
        }
        if (changesMask[12])
        {
            ConditionalTransmog.ReadUpdateMask(data);
        }
        if (changesMask[13])
        {
            SelfResSpells.ReadUpdateMask(data);
        }
        if (changesMask[14])
        {
            CharacterRestrictions.ReadUpdateMask(data);
        }
        if (changesMask[15])
        {
            SpellPctModByLabel.ReadUpdateMask(data);
        }
        if (changesMask[16])
        {
            SpellFlatModByLabel.ReadUpdateMask(data);
        }
    }
    if (changesMask[17])
    {
        for (std::size_t i = 0; i < 1; ++i)
        {
            if (changesMask[18 + i])
            {
                Research[i].ReadUpdateMask(data);
                for (std::size_t j = 0; j < Research[i].UpdatedSize(); ++j)
                {
                    if (Research[i].HasChanged(j))
                    {
                        Research[i][j].ReadUpdate(data);
                    }
                }
            }
        }
    }
    data.ResetBits();

    if (changesMask[0])
    {
        if (changesMask[2])
        {
            for (std::size_t i = 0; i < KnownTitles.UpdatedSize(); ++i)
            {
                if (KnownTitles.HasChanged(i))
                {
                    data >> KnownTitles[i];
                }
            }
        }
        if (changesMask[3])
        {
            for (std::size_t i = 0; i < ResearchSites.UpdatedSize(); ++i)
            {
                if (ResearchSites.HasChanged(i))
                {
                    data >> ResearchSites[i];
                }
            }
        }
        if (changesMask[4])
        {
            for (std::size_t i = 0; i < ResearchSiteProgress.UpdatedSize(); ++i)
            {
                if (ResearchSiteProgress.HasChanged(i))
                {
                    data >> ResearchSiteProgress[i];
                }
            }
        }
        if (changesMask[5])
        {
            for (std::size_t i = 0; i < DailyQuestsCompleted.UpdatedSize(); ++i)
            {
                if (DailyQuestsCompleted.HasChanged(i))
                {
                    data >> DailyQuestsCompleted[i];
                }
            }
        }
        if (changesMask[6])
        {
            for (std::size_t i = 0; i < AvailableQuestLineXQuestIDs.UpdatedSize(); ++i)
            {
                if (AvailableQuestLineXQuestIDs.HasChanged(i))
                {
                    data >> AvailableQuestLineXQuestIDs[i];
                }
            }
        }
        if (changesMask[7])
        {
            for (std::size_t i = 0; i < Unk254.UpdatedSize(); ++i)
            {
                if (Unk254.HasChanged(i))
                {
                    data >> Unk254[i];
                }
            }
        }
        if (changesMask[8])
        {
            for (std::size_t i = 0; i < Heirlooms.UpdatedSize(); ++i)
            {
                if (Heirlooms.HasChanged(i))
                {
                    data >> Heirlooms[i];
                }
            }
        }
        if (changesMask[9])
        {
            for (std::size_t i = 0; i < HeirloomFlags.UpdatedSize(); ++i)
            {
                if (HeirloomFlags.HasChanged(i))
                {
                    data >> HeirloomFlags[i];
                }
            }
        }
        if (changesMask[10])
        {
            for (std::size_t i = 0; i < Toys.UpdatedSize(); ++i)
            {
                if (Toys.HasChanged(i))
                {
                    data >> Toys[i];
                }
            }
        }
        if (changesMask[11])
        {
            for (std::size_t i = 0; i < Transmog.UpdatedSize(); ++i)
            {
                if (Transmog.HasChanged(i))
                {
                    data >> Transmog[i];
                }
            }
        }
        if (changesMask[12])
        {
            for (std::size_t i = 0; i < ConditionalTransmog.UpdatedSize(); ++i)
            {
                if (ConditionalTransmog.HasChanged(i))
                {
                    data >> ConditionalTransmog[i];
                }
            }
        }
        if (changesMask[13])
        {
            for (std::size_t i = 0; i < SelfResSpells.UpdatedSize(); ++i)
            {
                if (SelfResSpells.HasChanged(i))
                {
                    data >> SelfResSpells[i];
                }
            }
        }
        if (changesMask[15])
        {
            for (std::size_t i = 0; i < SpellPctModByLabel.UpdatedSize(); ++i)
            {
                if (SpellPctModByLabel.HasChanged(i))
                {
                    SpellPctModByLabel[i].ReadUpdate(data);
                }
            }
        }
        if (changesMask[16])
        {
            for (std::size_t i = 0; i < SpellFlatModByLabel.UpdatedSize(); ++i)
            {
                if (SpellFlatModByLabel.HasChanged(i))
                {
                    SpellFlatModByLabel[i].ReadUpdate(data);
                }
            }
        }
        if (changesMask[14])
        {
            for (std::size_t i = 0; i < CharacterRestrictions.UpdatedSize(); ++i)
            {
                if (CharacterRestrictions.HasChanged(i))
                {
                    CharacterRestrictions[i].ReadUpdate(data);
                }
            }
        }
        uint32 unknown;
        if (changesMask[19])
        {
            data >> unknown;
        }
        if (changesMask[20])
        {
            data >> unknown;
        }
        if (changesMask[21])
        {
            data >> unknown;
        }
        if (changesMask[22])
        {
            data >> unknown;
        }
        //if (changesMask[23])
        //{
        //    data >> unknown;
        //}
        //if (changesMask[24])
        //{
        //    data >> unknown;
        //}


        if (changesMask[23])
        {
            data >> FarsightObject._value;
        }
        if (changesMask[24])
        {
            data >> ComboTarget._value;
        }
        if (changesMask[25])
        {
            uint32 old = Coinage;
            data >> Coinage._value;
            update.OnActivePlayerCoinage(old, Coinage);
        }
        if (changesMask[26])//verified @3.4.2
        {
            data >> XP._value;
        }
        if (changesMask[27])
        {
            data >> NextLevelXP._value;
        }
        if (changesMask[28])
        {
            data >> TrialXP._value;
        }
        if (changesMask[29])
        {
            Skill._value.ReadUpdate(data, update);
        }
        if (changesMask[30])
        {
            data >> CharacterPoints._value;
        }
        if (changesMask[31])
        {
            data >> MaxTalentTiers._value;
        }
        if (changesMask[32])
        {
            data >> TrackCreatureMask._value;
        }
        if (changesMask[33])
        {
            data >> MainhandExpertise._value;
        }
    }
    if (changesMask[34])
    {
        /////////////////////////

        if (changesMask[35])
        {
            data >> OffhandExpertise._value;
        }
        if (changesMask[36])
        {
            data >> RangedExpertise._value;
        }
        if (changesMask[37])
        {
            data >> CombatRatingExpertise._value;
        }
        if (changesMask[38])
        {
            data >> BlockPercentage._value;
        }
        ////////////////////////
        if (changesMask[39])
        {
            data >> DodgePercentage._value;
        }
        if (changesMask[40])
        {
            data >> DodgePercentageFromAttribute._value;
        }
        if (changesMask[41])
        {
            data >> ParryPercentage._value;
        }
        if (changesMask[42])
        {
            data >> ParryPercentageFromAttribute._value;
        }
        if (changesMask[43])
        {
            data >> CritPercentage._value;
        }
        if (changesMask[44])
        {
            data >> RangedCritPercentage._value;
        }
        if (changesMask[45])
        {
            data >> OffhandCritPercentage._value;
        }
        if (changesMask[46])
        {
            data >> ShieldBlock._value;
        }
        if (changesMask[47])
        {
            data >> Mastery._value;
        }
        if (changesMask[48])
        {
            data >> Speed._value;
        }
        if (changesMask[49])
        {
            data >> Avoidance._value;
        }
        if (changesMask[50])
        {
            data >> Sturdiness._value;
        }
        if (changesMask[51])
        {
            data >> Unk340_3._value;
        }
        if (changesMask[52])
        {
            data >> Versatility._value;
        }
        if (changesMask[53])
        {
            data >> VersatilityBonus._value;
        }
        if (changesMask[54])
        {
            data >> PvpPowerDamage._value;
        }
        if (changesMask[55])
        {
            data >> PvpPowerHealing._value;
        }
        if (changesMask[56])
        {
            data >> ModHealingDonePos._value;
        }
        if (changesMask[57])
        {
            data >> ModHealingPercent._value;
        }
        if (changesMask[58])
        {
            data >> ModHealingDonePercent._value;
        }
        if (changesMask[59])
        {
            data >> ModPeriodicHealingDonePercent._value;
        }
        if (changesMask[60])
        {
            data >> ModSpellPowerPercent._value;
        }
        if (changesMask[61])
        {
            data >> ModResiliencePercent._value;
        }
        if (changesMask[62])
        {
            data >> OverrideSpellPowerByAPPercent._value;
        }
        if (changesMask[63])
        {
            data >> OverrideAPBySpellPowerPercent._value;
        }
        if (changesMask[64])
        {
            data >> ModTargetResistance._value;
        }
        if (changesMask[65])
        {
            data >> ModTargetPhysicalResistance._value;
        }
     }
    if (changesMask[66])
    {
        if (changesMask[67])
        {
            data >> LocalFlags._value;
        }
        if (changesMask[68])
        {
            data >> GrantableLevels._value;
        }
        if (changesMask[69])
        {
            data >> MultiActionBars._value;
        }
        if (changesMask[70])
        {
            data >> LifetimeMaxRank._value;
        }

        if (changesMask[71])
        {
            data >> NumRespecs._value;
        }
        if (changesMask[72])
        {
            data >> AmmoID._value;
        }
        if (changesMask[73])
        {
            data >> PvpMedals._value;
        }
        if (changesMask[74])
        {
            data >> TodayHonorableKills._value;
        }
        if (changesMask[75])
        {
            data >> TodayDishonorableKills._value;
        }
        if (changesMask[76])
        {
            data >> YesterdayHonorableKills._value;
        }
        if (changesMask[77])
        {
            data >> YesterdayDishonorableKills._value;
        }
        if (changesMask[78])
        {
            data >> LastWeekHonorableKills._value;
        }
        if (changesMask[79])
        {
            data >> LastWeekDishonorableKills._value;
        }
        if (changesMask[80])
        {
            data >> ThisWeekHonorableKills._value;
        }
        if (changesMask[81])
        {
            data >> ThisWeekDishonorableKills._value;
        }
        if (changesMask[82])
        {
            data >> ThisWeekContribution._value;
        }
        if (changesMask[83])
        {
            data >> LifetimeHonorableKills._value;
        }
        if (changesMask[84])
        {
            data >> LifetimeDishonorableKills._value;
        }
        if (changesMask[85])
        {
            data >> YesterdayContribution._value;
        }
        if (changesMask[86])
        {
            data >> LastWeekContribution._value;
        }
        if (changesMask[87])
        {
            data >> LastWeekRank._value;
        }
        if (changesMask[88])
        {
            data >> WatchedFactionIndex._value;
        }
        if (changesMask[89])
        {
            data >> MaxLevel._value;
        }
        if (changesMask[90])
        {
            data >> ScalingPlayerLevelDelta._value;
        }
        if (changesMask[91])
        {
            data >> MaxCreatureScalingLevel._value;
        }
        if (changesMask[92])
        {
            data >> PetSpellPower._value;
        }
        if (changesMask[93])
        {
            data >> UiHitModifier._value;
        }
        if (changesMask[94])
        {
            data >> UiSpellHitModifier._value;
        }
        if (changesMask[95])
        {
            data >> HomeRealmTimeOffset._value;
        }
        if (changesMask[96])
        {
            data >> ModPetHaste._value;
        }
        if (changesMask[97])
        {
            data >> LocalRegenFlags._value;
        }
    }
    if (changesMask[98])
    {
        /////////////////// my guess
        if (changesMask[99])
        {
            data >> AuraVision._value;
        }
        if (changesMask[100])
        {
            data >> NumBackpackSlots._value;
        }
        if (changesMask[101])
        {
            data >> OverrideSpellsID._value;
        }
        if (changesMask[102])
        {
            data >> LfgBonusFactionID._value;
        }
        ///////////////////////
        if (changesMask[103])
        {
            data >> LootSpecID._value;
        }
        if (changesMask[104])
        {
            data >> OverrideZonePVPType._value;
        }
        if (changesMask[105])
        {
            data >> Honor._value;
        }
        if (changesMask[106])
        {
            data >> HonorNextLevel._value;
        }
        if (changesMask[107])
        {
            data >> PvpTierMaxFromWins._value;
        }
        if (changesMask[108])
        {
            data >> PvpLastWeeksTierMaxFromWins._value;
        }
        if (changesMask[109])
        {
            data >> NumBankSlots._value;
        }

        if (changesMask[110])
        { 
            uint32 unknown;
            data >> unknown;
        }
        if (changesMask[111])
        {
            uint32 unknown;
            data >> unknown;
        }

        if (changesMask[112])//position verified @3.4.2
        {
            data >> TransportServerTime._value;
        }
        if (changesMask[113])
        {
            data >> GlyphsEnabled._value;
        }
        if (changesMask[114])
        {
            data >> Unk340._value;
        }
        if (changesMask[115])
        {
            uint32 unk;
            data >> unk;
        }
        /*if (changesMask[116])
        {
            uint32 unk;
            data >> unk;
        }
        if (changesMask[117])
        {
            uint32 unk;
            data >> unk;
        }*/
    }
     if (changesMask[116]) //position is verified @3.4.2
    {
        for (std::size_t i = 0; i < 129 + 12; ++i)
        {
            if (changesMask[117 + i])
            {
                uint32 old = InvSlots[i]._low;
                data >> InvSlots[i];
                update.OnActivePlayerInvSlot(old, InvSlots[i]._low, i);
            }
        }
    }
    if (changesMask[246])
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            if (changesMask[247 + i])
            {
                data >> TrackResourceMask[i];
            }
        }
    }
    //249->261 
    if (changesMask[261]) // ps is verified at @3.4.2
    {
        for (std::size_t i = 0; i < 7; ++i)
        {
            if (changesMask[262 + i])
            {
                data >> SpellCritPercentage[i];
            }
            if (changesMask[269 + i])
            {
                data >> ModDamageDonePos[i];
            }
            if (changesMask[276 + i])
            {
                data >> ModDamageDoneNeg[i];
            }
            if (changesMask[283 + i])
            {
                data >> ModDamageDonePercent[i];
            }
        }
    }
    if (changesMask[290])//ps is verified at @3.4.2
    {
        for (std::size_t i = 0; i < 240; ++i)
        {
            if (changesMask[291 + i])
            {
                data >> ExploredZones[i];
            }
        }
    }
    if (changesMask[531])//ps verified @3.4.2
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            if (changesMask[532 + i])
            {
                RestInfo[i].ReadUpdate(data);
            }
        }
    }
    if (changesMask[534])
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            if (changesMask[535 + i])
            {
                data >> WeaponDmgMultipliers[i];
            }
            if (changesMask[538 + i])
            {
                data >> WeaponAtkSpeedMultipliers[i];
            }
        }
    }
    if (changesMask[541])
    {
        for (std::size_t i = 0; i < 12; ++i)
        {
            if (changesMask[542 + i])
            {
                data >> BuybackPrice[i];
            }
            if (changesMask[554 + i])
            {
                data >> BuybackTimestamp[i];
            }
        }
    }
    if (changesMask[566])
    {
        for (std::size_t i = 0; i < 32; ++i)
        {
            if (changesMask[567 + i])
            {
                data >> CombatRatings[i];
            }
        }
    }
    if (changesMask[599])
    {
        for (std::size_t i = 0; i < 7; ++i)
        {
            if (changesMask[600 + i])
            {
                PvpInfo[i].ReadUpdate(data);
            }
        }
    }

    if (changesMask[607])
    {
        for (std::size_t i = 0; i < 4; ++i)
        {
            if (changesMask[608 + i])
            {
                data >> NoReagentCostMask[i];
            }
        }
    }
    if (changesMask[612])
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            if (changesMask[613 + i])
            {
                data >> ProfessionSkillLine[i];
            }
        }
    }
    if (changesMask[615])
    {
        for (std::size_t i = 0; i < 4; ++i)
        {
            if (changesMask[616 + i])
            {
                data >> BagSlotFlags[i];
            }
        }
    }
    if (changesMask[620])
    {
        for (std::size_t i = 0; i < 7; ++i)
        {
            if (changesMask[621 + i])
            {
                data >> BankBagSlotFlags[i];
            }
        }
    }
    //626->628 1 bits unknown
    if (changesMask[628])//@596 3.4.2
    {
        for (std::size_t i = 0; i < 875; ++i)
        {
            if (changesMask[629 + i])
            {
                data >> QuestCompleted[i];
                if (QuestCompleted[i])
                    update.OnActivePlayerQuestCompleted(0, QuestCompleted[i], i);
            }
        }
    }
    if (changesMask[1504])
    {
        for (std::size_t i = 0; i < 6; ++i)
        {
            if (changesMask[1505 + i])
            {
                GlyphInfos[i].ReadUpdate(data);
            }
        }
    }
    data.ResetBits();
}

void GameObjectData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> DisplayID._value;
    data >> SpellVisualID._value;
    data >> StateSpellVisualID._value;
    data >> SpawnTrackingStateAnimID._value;
    data >> SpawnTrackingStateAnimKitID._value;
    uint32 swesize = data.read<uint32>();
    for (std::size_t i = 0; i < swesize; ++i)
    {
        data >> EnsureCapasity(StateWorldEffectIDs._value, i);
    }
    data >> CreatedBy._value;
    data >> GuildGUID._value;
    data >> Flags._value;
    data >> ParentRotation._value.x;
    data >> ParentRotation._value.y;
    data >> ParentRotation._value.z;
    data >> ParentRotation._value.w;
    data >> FactionTemplate._value;
    data >> Level._value;
    data >> (uint8&)State._value;
    data >> (uint8&)TypeID._value;
    data >> PercentHealth._value;
    data >> ArtKit._value;
    uint32 edssize = data.read<uint32>();
    data >> CustomParam._value;
    uint32 wesize = data.read<uint32>();
    for (std::size_t i = 0; i < edssize; ++i)
    {
        data >> EnableDoodadSets[i];
    }
    for (std::size_t i = 0; i < wesize; ++i)
    {
        data >> WorldEffects[i];
    }
}

void GameObjectData::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<20> changesMask(data.ReadBits(20));

    if (changesMask[0])
    {
        if (changesMask[1])
        {
            uint32 size = data.ReadBits(32);
            for (std::size_t i = 0; i < size; ++i)
            {
                data >> EnsureCapasity(StateWorldEffectIDs._value, i);
            }
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[2])
        {
            EnableDoodadSets.ReadUpdateMask(data);
        }
        if (changesMask[3])
        {
            WorldEffects.ReadUpdateMask(data);
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[2])
        {
            for (std::size_t i = 0; i < EnableDoodadSets.UpdatedSize(); ++i)
            {
                if (EnableDoodadSets.HasChanged(i))
                {
                    data >> EnableDoodadSets[i];
                }
            }
        }
        if (changesMask[3])
        {
            for (std::size_t i = 0; i < WorldEffects.UpdatedSize(); ++i)
            {
                if (WorldEffects.HasChanged(i))
                {
                    data >> WorldEffects[i];
                }
            }
        }
        if (changesMask[4])
        {
            data >> DisplayID._value;
        }
        if (changesMask[5])
        {
            data >> SpellVisualID._value;
        }
        if (changesMask[6])
        {
            data >> StateSpellVisualID._value;
        }
        if (changesMask[7])
        {
            data >> SpawnTrackingStateAnimID._value;
        }
        if (changesMask[8])
        {
            data >> SpawnTrackingStateAnimKitID._value;
        }
        if (changesMask[9])
        {
            data >> CreatedBy._value;
        }
        if (changesMask[10])
        {
            data >> GuildGUID._value;
        }
        if (changesMask[11])
        {
            data >> Flags._value;
        }
        if (changesMask[12])
        {
            data >> ParentRotation._value.x;
            data >> ParentRotation._value.y;
            data >> ParentRotation._value.z;
            data >> ParentRotation._value.w;
        }
        if (changesMask[13])
        {
            data >> FactionTemplate._value;
        }
        if (changesMask[14])
        {
            data >> Level._value;
        }
        if (changesMask[15])
        {
            data >> (uint8&)State._value;
        }
        if (changesMask[16])
        {
            data >> (uint8&)TypeID._value;
        }
        if (changesMask[17])
        {
            data >> PercentHealth._value;
        }
        if (changesMask[18])
        {
            data >> ArtKit._value;
        }
        if (changesMask[19])
        {
            data >> CustomParam._value;
        }
    }
}

void DynamicObjectData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> Caster._value;
    data >> Type._value;
    data >> SpellXSpellVisualID._value;
    data >> SpellID._value;
    data >> Radius._value;
    data >> CastTime._value;
}

void DynamicObjectData::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<7> changesMask(data.ReadBits(7));

    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            data >> Caster._value;
        }
        if (changesMask[2])
        {
            data >> Type._value;
        }
        if (changesMask[3])
        {
            data >> SpellXSpellVisualID._value;
        }
        if (changesMask[4])
        {
            data >> SpellID._value;
        }
        if (changesMask[5])
        {
            data >> Radius._value;
        }
        if (changesMask[6])
        {
            data >> CastTime._value;
        }
    }
}

void CorpseData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> DynamicFlags._value;
    data >> Owner._value;
    data >> PartyGUID._value;
    data >> GuildGUID._value;
    data >> DisplayID._value;
    for (std::size_t i = 0; i < 19; ++i)
    {
        data >> Items[i];
    }
    data >> RaceID._value;
    data >> Sex._value;
    data >> Class._value;
    uint32 size = data.read<uint32>();
    data >> Flags._value;
    data >> FactionTemplate._value;
    for (std::size_t i = 0; i < size; ++i)
    {
        Customizations[i].ReadCreate(data, fieldVisibilityFlags);
    }
}

void CorpseData::ReadUpdate(ByteBuffer& data)
{
    if (!data.ReadBit())
        return;
    UpdateMask<32> changesMask(data.ReadBits(32));
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            Customizations.ReadUpdateMask(data);
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            for (std::size_t i = 0; i < Customizations.UpdatedSize(); ++i)
            {
                if (Customizations.HasChanged(i))
                {
                    Customizations[i].ReadUpdate(data);
                }
            }
        }
        if (changesMask[2])
        {
            data >> DynamicFlags._value;
        }
        if (changesMask[3])
        {
            data >> Owner._value;
        }
        if (changesMask[4])
        {
            data >> PartyGUID._value;
        }
        if (changesMask[5])
        {
            data >> GuildGUID._value;
        }
        if (changesMask[6])
        {
            data >> DisplayID._value;
        }
        if (changesMask[7])
        {
            data >> RaceID._value;
        }
        if (changesMask[8])
        {
            data >> Sex._value;
        }
        if (changesMask[9])
        {
            data >> Class._value;
        }
        if (changesMask[10])
        {
            data >> Flags._value;
        }
        if (changesMask[11])
        {
            data >> FactionTemplate._value;
        }
    }
    if (changesMask[12])
    {
        for (std::size_t i = 0; i < 19; ++i)
        {
            if (changesMask[13 + i])
            {
                data >> Items[i];
            }
        }
    }
}

void ScaleCurve::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> StartTimeOffset._value;
    for (std::size_t i = 0; i < 2; ++i)
    {
        data >> Points[i].Pos.m_positionX;
        data >> Points[i].Pos.m_positionY;
    }
    data >> ParameterCurve._value;
    (bool&)OverrideActive = data.ReadBit();
    data.ResetBits();
}

void ScaleCurve::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<7> changesMask(data.ReadBits(7));
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            (bool&)OverrideActive = data.ReadBit();
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[2])
        {
            data >> StartTimeOffset._value;
        }
        if (changesMask[3])
        {
            data >> ParameterCurve._value;
        }
    }
    if (changesMask[4])
    {
        for (std::size_t i = 0; i < 2; ++i)
        {
            if (changesMask[5 + i])
            {
                data >> Points[i];
            }
        }
    }
    data.ResetBits();
}

void VisualAnim::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> AnimationDataID._value;
    data >> AnimKitID._value;
    data >> AnimProgress._value;
    data >> Field_C._value;
}
void VisualAnim::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<5> changesMask(data.ReadBits(5));
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            data >> Field_C._value;
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[2])
        {
            data >> AnimationDataID._value;
        }
        if (changesMask[3])
        {
            data >> AnimKitID._value;
        }
        if (changesMask[4])
        {
            data >> AnimProgress._value;
        }
    }
}

void AreaTriggerData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    OverrideScaleCurve._value.ReadCreate(data, fieldVisibilityFlags);
    data >> Caster._value;
    data >> Duration._value;
    data >> TimeToTarget._value;
    data >> TimeToTargetScale._value;
    data >> TimeToTargetExtraScale._value;
    data >> SpellID._value;
    data >> SpellForVisuals._value;
    data >> SpellXSpellVisualID._value;
    data >> BoundsRadius2D._value;
    data >> DecalPropertiesID._value;
    data >> CreatingEffectGUID._value;
    data >> Field_80._value;
    ExtraScaleCurve._value.ReadCreate(data, fieldVisibilityFlags);
    VisualAnim._value.ReadCreate(data, fieldVisibilityFlags);
}

void AreaTriggerData::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<16> changesMask(data.ReadBits(16));
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            OverrideScaleCurve._value.ReadUpdate(data);
        }
        if (changesMask[3])
        {
            data >> Caster._value;
        }
        if (changesMask[4])
        {
            data >> Duration._value;
        }
        if (changesMask[5])
        {
            data >> TimeToTarget._value;
        }
        if (changesMask[6])
        {
            data >> TimeToTargetScale._value;
        }
        if (changesMask[7])
        {
            data >> TimeToTargetExtraScale._value;
        }
        if (changesMask[8])
        {
            data >> SpellID._value;
        }
        if (changesMask[9])
        {
            data >> SpellForVisuals._value;
        }
        if (changesMask[10])
        {
            data >> SpellXSpellVisualID._value;
        }
        if (changesMask[11])
        {
            data >> BoundsRadius2D._value;
        }
        if (changesMask[12])
        {
            data >> DecalPropertiesID._value;
        }
        if (changesMask[13])
        {
            data >> CreatingEffectGUID._value;
        }
        if (changesMask[14])
        {
            data >> Field_80._value;
        }
        if (changesMask[2])
        {
            ExtraScaleCurve._value.ReadUpdate(data);
        }
        if (changesMask[15])
        {
            VisualAnim._value.ReadUpdate(data);
        }
    }
}

void SceneObjectData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> ScriptPackageID._value;
    data >> RndSeedVal._value;
    data >> CreatedBy._value;
    data >> SceneType._value;
}

void SceneObjectData::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<5> changesMask(data.ReadBits(5));
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[1])
        {
            data >> ScriptPackageID._value;
        }
        if (changesMask[2])
        {
            data >> RndSeedVal._value;
        }
        if (changesMask[3])
        {
            data >> CreatedBy._value;
        }
        if (changesMask[4])
        {
            data >> SceneType._value;
        }
    }
}

void ConversationLine::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    data >> ConversationLineID;
    data >> StartTime;
    data >> UiCameraID;
    data >> ActorIndex;
    data >> Flags;
}

void ConversationLine::ReadUpdate(ByteBuffer& data)
{
    data >> ConversationLineID;
    data >> StartTime;
    data >> UiCameraID;
    data >> ActorIndex;
    data >> Flags;
}

void ConversationActor::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    Type = data.ReadBit();
    data.ReadBit();
    data >> Id;
    data >> ActorGUID;
    data >> CreatureID;
    data >> CreatureDisplayInfoID;
}

void ConversationActor::ReadUpdate(ByteBuffer& data)
{
    Type = data.ReadBit();
    data.ReadBit();
    data >> Id;
    data >> ActorGUID;
    data >> CreatureID;
    data >> CreatureDisplayInfoID;
}

void ConversationData::ReadCreate(ByteBuffer& data, EnumFlag<UpdateFieldFlag> fieldVisibilityFlags)
{
    uint32 size = data.read<uint32>();
    data >> LastLineEndTime._value;
    for (std::size_t i = 0; i < size; ++i)
    {
        EnsureCapasity(Lines._value, i).ReadCreate(data, fieldVisibilityFlags);
    }
    data >> Progress._value;
    data >> size;
    for (std::size_t i = 0; i < size; ++i)
    {
        Actors[i].ReadCreate(data, fieldVisibilityFlags);
    }
}


void ConversationData::ReadUpdate(ByteBuffer& data)
{
    UpdateMask<5> changesMask(data.ReadBits(5));

    if (changesMask[0])
    {
        if (changesMask[1])
        {
            uint32 size = data.ReadBits(32);
            for (std::size_t i = 0; i < size; ++i)
            {
                EnsureCapasity(Lines._value, i).ReadUpdate(data);
            }
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[2])
        {
                Actors.ReadUpdateMask(data);
        }
    }
    data.ResetBits();
    if (changesMask[0])
    {
        if (changesMask[2])
        {
            for (std::size_t i = 0; i < Actors.UpdatedSize(); ++i)
            {
                if (Actors.HasChanged(i))
                {
                    Actors[i].ReadUpdate(data);
                }
            }
        }
        if (changesMask[3])
        {
            data >> LastLineEndTime._value;
        }
        if (changesMask[4])
        {
            data >> Progress._value;
        }
    }
}

}
