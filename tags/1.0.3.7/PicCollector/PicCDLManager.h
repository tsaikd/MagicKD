#pragma once
#include "KDGetHttpFile.h"
#include "PicCFeed.h"

class CPicCDLManager : public CKDGetHttpFile
{
public:
	CPicCDLManager();
	virtual ~CPicCDLManager();

	void Init(CPicCFeed *pFeed);
	void Destroy();

	bool AddFileListQuick(LPCTSTR lpURL, LPCTSTR lpLocalPath);
	void SetDBSync(bool bSync = true);

	void DelayDownload();

protected:
	virtual void OnStartDownloadThread();
	virtual void OnExitDownloadThread();
	virtual void OnDownloadFileOver();
	virtual void OnDownloadFileDiscard();
	virtual void OnDownloadFileRetryFailed();

private:
	CPicCFeed *m_pFeed;
	bool m_bDelay;
};
