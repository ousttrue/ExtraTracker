#pragma once
#include <openvr_driver.h>
#include "driverlog.h"

class CSampleDeviceDriver : public vr::ITrackedDeviceServerDriver
{
public:
    CSampleDeviceDriver();
    virtual ~CSampleDeviceDriver();
    virtual vr::EVRInitError Activate(vr::TrackedDeviceIndex_t unObjectId);
    virtual void Deactivate()
    {
        m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
    }
    virtual void EnterStandby()
    {
    }
    void *GetComponent(const char *pchComponentNameAndVersion)
    {
        if (!_stricmp(pchComponentNameAndVersion, vr::IVRDisplayComponent_Version))
        {
            return m_display;
        }

        // override this to add a component to a driver
        return NULL;
    }

    /** debug request from a client */
    virtual void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize)
    {
        if (unResponseBufferSize >= 1)
            pchResponseBuffer[0] = 0;
    }

    virtual vr::DriverPose_t GetPose();

    void RunFrame();

    std::string GetSerialNumber() const { return m_sSerialNumber; }

private:
    vr::TrackedDeviceIndex_t m_unObjectId;
    vr::PropertyContainerHandle_t m_ulPropertyContainer;

    std::string m_sSerialNumber;
    std::string m_sModelNumber;

    class CDisplay *m_display;
};
