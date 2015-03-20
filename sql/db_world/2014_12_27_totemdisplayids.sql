
CREATE TABLE IF NOT EXISTS `totemdisplayids` (
  `displayid` int(10) unsigned NOT NULL DEFAULT '0',
  `draeneiid` int(10) unsigned NOT NULL DEFAULT '0',
  `dwarfid` int(10) unsigned NOT NULL DEFAULT '0',
  `goblinid` int(10) unsigned NOT NULL DEFAULT '0',
  `orcid` int(10) unsigned NOT NULL DEFAULT '0',
  `taurenid` int(10) unsigned NOT NULL DEFAULT '0',
  `trollid` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`displayid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

REPLACE INTO `totemdisplayids` (`displayid`, `draeneiid`, `dwarfid`, `goblinid`, `orcid`, `taurenid`, `trollid`) VALUES
	(4587, 19075, 30755, 30784, 30759, 4587, 30763),
	(4588, 19073, 30753, 30782, 30757, 4588, 30761),
	(4589, 19074, 30754, 30783, 30758, 4589, 30762),
	(4590, 19071, 30736, 30781, 30756, 4590, 30760);
