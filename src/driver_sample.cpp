//============ Copyright (c) Valve Corporation, All rights reserved. ============

#include <openvr_driver.h>
#include "driverlog.h"

#include <vector>
#include <thread>
#include <chrono>

#if defined(_WINDOWS)
#include <windows.h>
#endif

using namespace vr;

#if defined(_WIN32)
#define HMD_DLL_EXPORT extern "C" __declspec(dllexport)
#define HMD_DLL_IMPORT extern "C" __declspec(dllimport)
#elif defined(__GNUC__) || defined(COMPILER_GCC) || defined(__APPLE__)
#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#define HMD_DLL_IMPORT extern "C"
#else
#error "Unsupported Platform."
#endif

#include "helper.h"

#include "watchdog.h"
#include "trackeddevice.h"
#include "controller.h"

CWatchdogDriver_Sample g_watchdogDriverNull;

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CServerDriver_Sample : public IServerTrackedDeviceProvider
{
public:
	virtual EVRInitError Init(vr::IVRDriverContext *pDriverContext);
	virtual void Cleanup();
	virtual const char *const *GetInterfaceVersions() { return vr::k_InterfaceVersions; }
	virtual void RunFrame();
	virtual bool ShouldBlockStandbyMode() { return false; }
	virtual void EnterStandby() {}
	virtual void LeaveStandby() {}

private:
	CSampleDeviceDriver *m_pNullHmdLatest = nullptr;
	CSampleControllerDriver *m_pController = nullptr;
};

CServerDriver_Sample g_serverDriverNull;

EVRInitError CServerDriver_Sample::Init(vr::IVRDriverContext *pDriverContext)
{
	VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
	InitDriverLog(vr::VRDriverLog());

	m_pNullHmdLatest = new CSampleDeviceDriver();
	vr::VRServerDriverHost()->TrackedDeviceAdded(m_pNullHmdLatest->GetSerialNumber().c_str(), vr::TrackedDeviceClass_HMD, m_pNullHmdLatest);

	m_pController = new CSampleControllerDriver();
	vr::VRServerDriverHost()->TrackedDeviceAdded(m_pController->GetSerialNumber().c_str(), vr::TrackedDeviceClass_Controller, m_pController);

	return VRInitError_None;
}

void CServerDriver_Sample::Cleanup()
{
	CleanupDriverLog();
	delete m_pNullHmdLatest;
	m_pNullHmdLatest = NULL;
	delete m_pController;
	m_pController = NULL;
}

void CServerDriver_Sample::RunFrame()
{
	if (m_pNullHmdLatest)
	{
		m_pNullHmdLatest->RunFrame();
	}
	if (m_pController)
	{
		m_pController->RunFrame();
	}

	vr::VREvent_t vrEvent;
	while (vr::VRServerDriverHost()->PollNextEvent(&vrEvent, sizeof(vrEvent)))
	{
		if (m_pController)
		{
			m_pController->ProcessEvent(vrEvent);
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
HMD_DLL_EXPORT void *HmdDriverFactory(const char *pInterfaceName, int *pReturnCode)
{

	if (0 == strcmp(IServerTrackedDeviceProvider_Version, pInterfaceName))
	{
		return &g_serverDriverNull;
	}
	if (0 == strcmp(IVRWatchdogProvider_Version, pInterfaceName))
	{
		return &g_watchdogDriverNull;
	}

	if (pReturnCode)
		*pReturnCode = VRInitError_Init_InterfaceNotFound;

	return NULL;
}
