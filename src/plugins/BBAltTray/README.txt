=========
BBAltTray
=========


Abstract
========

BBAltTray is a plugin for Blackbox for Windows <http://bb4win.org/>.
It provides the way to access the system tray by bro@ms.




Install / Uninstall
===================

Same as other plugins.


Install
-------

1. Copy BBAltTray.dll to anywhere you want.

2. Add the path to BBAltTray.dll in your plugins.rc.

3. Restart the shell.


Uninstall
---------

1. Delete the path to BBAltTray.dll in your plugins.rc.

2. Restart the shell.

3. Delete BBAltTray.dll.




Bro@ms
======

@BBAltTray.Next
	Select the next icon.

@BBAltTray.Prev
	Select the previous icon.

@BBAltTray.Cancel
	Cancel selecting icon.

@BBAltTray.Send <message>
	Send <message> to the selected icon.
	<message> is one of the following values:

	* LeftClick
	* RightClick
	* MiddleClick
	* LeftDoubleClick
	* RightDoubleClick
	* MiddleDoubleClick




BBAltTray.rc
============

bbalttray.base_padding: 0 0 0 0
	Specifies the amount of padding of the entire window in pixels.
	This value consists of 4 integers
	that are the amount of the top, right, bottom and left padding.

bbalttray.base_style: menu.frame
	Specifies the gradient style of the entire window.
	Available values are: menu.title, menu.frame, menu.hilite,
	toolbar, toolbar.label, toolbar.windowlabel and toolbar.clock.

bbalttray.base_border: false
	Specifies whether to draw the border of the entire window.

bbalttray.tip_padding: 2 13 2 4
	Specifies the amount of padding of an icon/tip in pixels.

bbalttray.tip_style: menu.hilite
	Specifies the gradient style of the selected icon.

bbalttray.tip_border: false
	Specifies whether to draw the border of the selected icon/tip.

bbalttray.icon_tip_padding: 4
	Specifies the amount of padding of between icon and tip in pixels.

bbalttray.icon_size: 16
	Specifies the size of icon in pixels.
	If this value is zero, BBAltTray will not draw icons.

bbalttray.do_convert_new_line: true
	Specifies whether to convert new line characters in tips.

bbalttray.converted_new_line: " / "
	Replacement string of new line characters.




ChangeLog
=========

0.0.2	2005-04-06
	- Modified to clean dead tray icons.

	- Added feature to convert new line characters in tips
	  (bbalttray.do_convert_new_line, bbalttray.converted_new_line).

0.0.1	2004-12-25
	- Fixed the bug that the focus sometimes goes somewhere
	  other than the menu of an icon when `@BBAltTray.Send' is sent.
	  This bug denies to access the menu by keyboard.

0.0.0	2004-12-20
	- Initial version.




License
=======

BBAltTray is released under the terms of GNU General Public License
either version 2 or (at your option) any later version.




Misc.
=====

I've tested this plugin on bbLean 1.13b3 / Windows XP Pro SP2.
I guess that this plugin may work well
on other shells and/or other version of Windows.

For latest information, see <http://nicht.s8.xrea.com/2004/12/BBAltTray>.

Copyright (C) 2004 kana <nicht AT s8 DOT xrea DOT con>

[END]
