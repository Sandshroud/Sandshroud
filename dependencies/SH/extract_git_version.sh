#!/bin/sh
version=`git log -1 --pretty=format:"%h"`
timestamp=`git log -1 --pretty=format:"%ct"`
tag=`git log -1 --pretty=format:"%d" | cut -d ' ' -f3 | cut -d ')' -f1`
hostname=`hostname`
username=`whoami | cut -d\\\ -f2`

if [ -z "$version" ]
then
	version="0"
fi

if [ -z "$timestamp" ]
then
	timestamp="0"
fi


echo "// Do not alter, automatically extracted." > git_version.h
echo "" >> git_version.h
echo "#pragma once" >> git_version.h
echo "" >> git_version.h
echo "#define BUILD_TAG \"$tag\"" >> git_version.h
echo "#define BUILD_HASH 0x$version" >> git_version.h
echo "#define COMMIT_TIMESTAMP $timestamp" >> git_version.h
echo "#define BUILD_HASH_STR \"$version\"" >> git_version.h
echo "#define BUILD_USER_STR \"$username\"" >> git_version.h
echo "#define BUILD_HOST_STR \"$hostname\"" >> git_version.h
echo "" >> git_version.h
