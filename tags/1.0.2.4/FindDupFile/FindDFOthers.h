#pragma once
#include "FindDupFileProc.h"

typedef CList<CFindDupFileProc *, CFindDupFileProc *> CListFindDupFileProc;

int FileTimeCmp(FILETIME *pft1, FILETIME *pft2);
int FileCmpCB(const void *pv1, const void *pv2);

void MergeListFindDupFileProc(CFindDupFileProc *p1, CFindDupFileProc *p2);
