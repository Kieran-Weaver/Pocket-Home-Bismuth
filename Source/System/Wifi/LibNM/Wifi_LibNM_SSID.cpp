#include "Wifi_LibNM_SSID.h"
#include <NetworkManager.h>

// Creates this SSID copying another SSID.
Wifi::LibNM::SSID::SSID(const SSID& toCopy)
{
    copyByteArray(toCopy.getByteArray());
}


// Creates this SSID from a temporary SSID object.
Wifi::LibNM::SSID::SSID(SSID&& toCopy)
{
    ssidBytes = toCopy.ssidBytes;
    toCopy.ssidBytes = nullptr;
}


// Creates this SSID from a SSID byte string.
Wifi::LibNM::SSID::SSID(GBytes* toCopy)
{
    copyByteArray(toCopy);
}


// Frees all SSID data on destruction.
Wifi::LibNM::SSID::~SSID()
{
    clearByteArray();
}


// Gets a text representation of this SSID.
juce::String Wifi::LibNM::SSID::toString() const
{
    juce::String ssidText;
    if (ssidBytes != nullptr)
    {
        gsize len = 0;
        const guint8* data = (const guint8*)g_bytes_get_data( ssidBytes, &len );
        char* utfSSID = nm_utils_ssid_to_utf8(data, len);
        if (utfSSID != nullptr)
        {
            ssidText = utfSSID;
            g_free(utfSSID);
            utfSSID = nullptr;
        }
    }
    return ssidText;
}


// Gets this SSID's internal SSID byte string.
GBytes* Wifi::LibNM::SSID::getByteArray() const
{
    return ssidBytes;
}


// Assigns another SSID's data to this SSID.
Wifi::LibNM::SSID& Wifi::LibNM::SSID::operator= (const SSID& toCopy)
{
    copyByteArray(toCopy.getByteArray());
    return *this;
}


// Assigns a copy of an SSID bytestring to this SSID.
Wifi::LibNM::SSID& Wifi::LibNM::SSID::operator= (GBytes* toAssign)
{
    copyByteArray(toAssign);
    return *this;
}


// Checks if two SSIDs are equivalent.
bool Wifi::LibNM::SSID::operator== (const SSID& rhs) const
{
    return *this == rhs.getByteArray();
}


// Checks if two SSIDs are not equivalent.
bool Wifi::LibNM::SSID::operator!= (const SSID& rhs) const
{
    return ! (*this == rhs.getByteArray());
}


// Compares this object with another SSID alphabetically.
bool Wifi::LibNM::SSID::operator< (const SSID& rhs) const
{
    if (ssidBytes == nullptr)
    {
        return false;
    }
    if (rhs.ssidBytes == nullptr)
    {
        return true;
    }
    return !!g_bytes_compare(ssidBytes, rhs.ssidBytes);
}


// Checks if a SSID and a raw SSID byte string are equivalent.
bool Wifi::LibNM::SSID::operator== (GBytes* rhs) const
{
    if (ssidBytes == nullptr)
    {
        return rhs == nullptr;
    }
    if (rhs == nullptr)
    {
        return ssidBytes == nullptr;
    }
    return g_bytes_equal( ssidBytes, rhs );
}


// Checks if a SSID and a raw SSID byte string are not equivalent.
bool Wifi::LibNM::SSID::operator!= (GBytes* rhs) const
{
    return ! (*this == rhs);
}


// Copies a SSID byte string, storing it in this object.
void Wifi::LibNM::SSID::copyByteArray(GBytes* toCopy)
{
    clearByteArray();
    if (toCopy != nullptr)
    {
        ssidBytes = g_bytes_ref( toCopy );
    }
}


// Frees any non-null SSID byte string stored in this object.
void Wifi::LibNM::SSID::clearByteArray()
{
    if (ssidBytes != nullptr)
    {
        g_bytes_unref(ssidBytes);
        ssidBytes = nullptr;
    }
}
