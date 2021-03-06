/*
 * This file is part of Compare Plugin for Notepad++
 * Copyright (C)2017-2019 Pavel Nedev (pg.nedev@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Compare.h"
#include "UserSettings.h"

#include <shlwapi.h>
#include <cstdlib>


const TCHAR UserSettings::mainSection[]					= TEXT("Main Settings");

const TCHAR UserSettings::oldIsFirstSetting[]			= TEXT("Old is First");
const TCHAR UserSettings::oldFileViewSetting[]			= TEXT("Old in Sub View");
const TCHAR UserSettings::compareToPrevSetting[]		= TEXT("Default Compare is to Prev");

const TCHAR UserSettings::alignAllMatchesSetting[]		= TEXT("Align All Matches");
const TCHAR UserSettings::markIgnoredLinesSetting[]		= TEXT("Never Mark Ignored Lines");
const TCHAR UserSettings::encodingsCheckSetting[]		= TEXT("Check Encodings");
const TCHAR UserSettings::promptCloseOnMatchSetting[]	= TEXT("Prompt to Close on Match");
const TCHAR UserSettings::wrapAroundSetting[]			= TEXT("Wrap Around");
const TCHAR UserSettings::gotoFirstDiffSetting[]		= TEXT("Go to First Diff");
const TCHAR UserSettings::followingCaretSetting[]		= TEXT("Following Caret");

const TCHAR UserSettings::charPrecisionSetting[]		= TEXT("Character Level Highlight");
const TCHAR UserSettings::ignoreSpacesSetting[]			= TEXT("Ignore Spaces");
const TCHAR UserSettings::ignoreEmptyLinesSetting[]		= TEXT("Ignore Empty Lines");
const TCHAR UserSettings::ignoreCaseSetting[]			= TEXT("Ignore Case");
const TCHAR UserSettings::detectMovesSetting[]			= TEXT("Detect Moves");
const TCHAR UserSettings::showOnlySelSetting[]			= TEXT("Show Only Selections");
const TCHAR UserSettings::showOnlyDiffSetting[]			= TEXT("Show Only Diffs");
const TCHAR UserSettings::navBarSetting[]				= TEXT("Navigation Bar");

const TCHAR UserSettings::reCompareOnChangeSetting[]	= TEXT("Re-Compare on Change");

const TCHAR UserSettings::statusTypeSetting[]			= TEXT("Status Type");

const TCHAR UserSettings::colorsSection[]				= TEXT("Color Settings");

const TCHAR UserSettings::addedColorSetting[]			= TEXT("Added");
const TCHAR UserSettings::removedColorSetting[]			= TEXT("Removed");
const TCHAR UserSettings::changedColorSetting[]			= TEXT("Changed");
const TCHAR UserSettings::movedColorSetting[]			= TEXT("Moved");
const TCHAR UserSettings::highlightColorSetting[]		= TEXT("Highlight");
const TCHAR UserSettings::highlightTranspSetting[]		= TEXT("Transparency");
const TCHAR UserSettings::changedThresholdSetting[]		= TEXT("Changed Threshold Percentage");


void UserSettings::load()
{
	TCHAR iniFile[MAX_PATH];

	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, (WPARAM)_countof(iniFile), (LPARAM)iniFile);

	::PathAppend(iniFile, TEXT("ComparePlugin.ini"));

	OldFileIsFirst			= ::GetPrivateProfileInt(mainSection, oldIsFirstSetting,
			DEFAULT_OLD_IS_FIRST, iniFile) != 0;
	OldFileViewId			= ::GetPrivateProfileInt(mainSection, oldFileViewSetting,
			DEFAULT_OLD_IN_SUB_VIEW, iniFile) == 0 ? MAIN_VIEW : SUB_VIEW;
	CompareToPrev			= ::GetPrivateProfileInt(mainSection, compareToPrevSetting,
			DEFAULT_COMPARE_TO_PREV, iniFile) != 0;
	AlignAllMatches			= ::GetPrivateProfileInt(mainSection, alignAllMatchesSetting,
			DEFAULT_ALIGN_ALL_MATCHES, iniFile) != 0;
	NeverMarkIgnored		= ::GetPrivateProfileInt(mainSection, markIgnoredLinesSetting,
			DEFAULT_NEVER_MARK_IGNORED, iniFile) != 0;
	EncodingsCheck			= ::GetPrivateProfileInt(mainSection, encodingsCheckSetting,
			DEFAULT_ENCODINGS_CHECK, iniFile) != 0;
	FollowingCaret			= ::GetPrivateProfileInt(mainSection, followingCaretSetting,
			DEFAULT_FOLLOWING_CARET, iniFile) != 0;
	WrapAround				= ::GetPrivateProfileInt(mainSection, wrapAroundSetting,
			DEFAULT_WRAP_AROUND, iniFile) != 0;
	GotoFirstDiff			= ::GetPrivateProfileInt(mainSection, gotoFirstDiffSetting,
			DEFAULT_GOTO_FIRST_DIFF, iniFile) != 0;
	PromptToCloseOnMatch	= ::GetPrivateProfileInt(mainSection, promptCloseOnMatchSetting,
			DEFAULT_PROMPT_CLOSE_ON_MATCH, iniFile) != 0;

	CharPrecision		= ::GetPrivateProfileInt(mainSection, charPrecisionSetting,		0, iniFile) != 0;
	IgnoreSpaces		= ::GetPrivateProfileInt(mainSection, ignoreSpacesSetting,		0, iniFile) != 0;
	IgnoreEmptyLines	= ::GetPrivateProfileInt(mainSection, ignoreEmptyLinesSetting,	0, iniFile) != 0;
	IgnoreCase			= ::GetPrivateProfileInt(mainSection, ignoreCaseSetting,		0, iniFile) != 0;
	DetectMoves			= ::GetPrivateProfileInt(mainSection, detectMovesSetting,		1, iniFile) != 0;
	ShowOnlyDiffs		= ::GetPrivateProfileInt(mainSection, showOnlyDiffSetting,		0, iniFile) != 0;
	ShowOnlySelections	= ::GetPrivateProfileInt(mainSection, showOnlySelSetting,		1, iniFile) != 0;
	UseNavBar			= ::GetPrivateProfileInt(mainSection, navBarSetting,			1, iniFile) != 0;

	RecompareOnChange	= ::GetPrivateProfileInt(mainSection, reCompareOnChangeSetting,	1, iniFile) != 0;

	SavedStatusType	= static_cast<StatusType>(::GetPrivateProfileInt(mainSection, statusTypeSetting,
			DEFAULT_STATUS_TYPE, iniFile));

	statusType = (SavedStatusType < STATUS_TYPE_END) ? SavedStatusType : static_cast<StatusType>(DEFAULT_STATUS_TYPE);

	colors.added		= ::GetPrivateProfileInt(colorsSection, addedColorSetting,
			DEFAULT_ADDED_COLOR, iniFile);
	colors.deleted		= ::GetPrivateProfileInt(colorsSection, removedColorSetting,
			DEFAULT_DELETED_COLOR, iniFile);
	colors.changed		= ::GetPrivateProfileInt(colorsSection, changedColorSetting,
			DEFAULT_CHANGED_COLOR, iniFile);
	colors.moved		= ::GetPrivateProfileInt(colorsSection, movedColorSetting,
			DEFAULT_MOVED_COLOR, iniFile);
	colors.highlight	= ::GetPrivateProfileInt(colorsSection, highlightColorSetting,
			DEFAULT_HIGHLIGHT_COLOR, iniFile);
	colors.transparency	= ::GetPrivateProfileInt(colorsSection, highlightTranspSetting,
			DEFAULT_HIGHLIGHT_TRANSP, iniFile);

	ChangedThresholdPercent	= ::GetPrivateProfileInt(colorsSection, changedThresholdSetting,
			DEFAULT_CHANGED_THRESHOLD, iniFile);

	dirty = false;
}


void UserSettings::save()
{
	if (!dirty && (SavedStatusType == statusType))
		return;

	TCHAR iniFile[MAX_PATH];

	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, (WPARAM)_countof(iniFile), (LPARAM)iniFile);

	if (::PathFileExists(iniFile) == FALSE)
	{
		if (::CreateDirectory(iniFile, NULL) == FALSE)
		{
			TCHAR msg[MAX_PATH + 128];

			_sntprintf_s(msg, _countof(msg), _TRUNCATE,
					TEXT("Notepad++ plugins config folder\n'%s'\ndoesn't exist and failed to be created.")
					TEXT("\nCannot write configuration file."), iniFile);
			::MessageBox(nppData._nppHandle, msg, PLUGIN_NAME, MB_OK | MB_ICONWARNING);

			return;
		}
	}

	::PathAppend(iniFile, TEXT("ComparePlugin.ini"));

	if (!::WritePrivateProfileString(mainSection, oldIsFirstSetting,
			OldFileIsFirst ? TEXT("1") : TEXT("0"), iniFile))
	{
		TCHAR msg[MAX_PATH + 64];

		_sntprintf_s(msg, _countof(msg), _TRUNCATE, TEXT("Failed to write\n'%s'\nconfiguration file."), iniFile);
		::MessageBox(nppData._nppHandle, msg, PLUGIN_NAME, MB_OK | MB_ICONWARNING);

		return;
	}

	::WritePrivateProfileString(mainSection, oldFileViewSetting,
			OldFileViewId == SUB_VIEW ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, compareToPrevSetting,
			CompareToPrev ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, alignAllMatchesSetting,
			AlignAllMatches ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, markIgnoredLinesSetting,
			NeverMarkIgnored ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, encodingsCheckSetting,
			EncodingsCheck ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, followingCaretSetting,
			FollowingCaret ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, wrapAroundSetting,
			WrapAround ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, gotoFirstDiffSetting,
			GotoFirstDiff ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, promptCloseOnMatchSetting,
			PromptToCloseOnMatch ? TEXT("1") : TEXT("0"), iniFile);

	::WritePrivateProfileString(mainSection, charPrecisionSetting,
			CharPrecision ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, ignoreSpacesSetting,
			IgnoreSpaces ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, ignoreEmptyLinesSetting,
			IgnoreEmptyLines ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, ignoreCaseSetting,
			IgnoreCase ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, detectMovesSetting,
			DetectMoves ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, showOnlyDiffSetting,
			ShowOnlyDiffs ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, showOnlySelSetting,
			ShowOnlySelections ? TEXT("1") : TEXT("0"), iniFile);
	::WritePrivateProfileString(mainSection, navBarSetting,
			UseNavBar ? TEXT("1") : TEXT("0"), iniFile);

	::WritePrivateProfileString(mainSection, reCompareOnChangeSetting,
			RecompareOnChange ? TEXT("1") : TEXT("0"), iniFile);

	TCHAR buffer[64];

	SavedStatusType = statusType;

	_itot_s(static_cast<int>(SavedStatusType), buffer, 64, 10);
	::WritePrivateProfileString(mainSection, statusTypeSetting, buffer, iniFile);

	_itot_s(colors.added, buffer, 64, 10);
	::WritePrivateProfileString(colorsSection, addedColorSetting, buffer, iniFile);

	_itot_s(colors.deleted, buffer, 64, 10);
	::WritePrivateProfileString(colorsSection, removedColorSetting, buffer, iniFile);

	_itot_s(colors.changed, buffer, 64, 10);
	::WritePrivateProfileString(colorsSection, changedColorSetting, buffer, iniFile);

	_itot_s(colors.moved, buffer, 64, 10);
	::WritePrivateProfileString(colorsSection, movedColorSetting, buffer, iniFile);

	_itot_s(colors.highlight, buffer, 64, 10);
	::WritePrivateProfileString(colorsSection, highlightColorSetting, buffer, iniFile);

	_itot_s(colors.transparency, buffer, 64, 10);
	::WritePrivateProfileString(colorsSection, highlightTranspSetting, buffer, iniFile);

	_itot_s(ChangedThresholdPercent, buffer, 64, 10);
	::WritePrivateProfileString(colorsSection, changedThresholdSetting, buffer, iniFile);

	dirty = false;
}
