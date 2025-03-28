// SPDX-License-Identifier: LicenseRef-AGPL-3.0-only-OpenSSL

#ifndef CHIAKI_HOST_H
#define CHIAKI_HOST_H

#include <chiaki/regist.h>

#include <QMetaType>
#include <QString>

class QSettings;

class HostMAC
{
	private:
		uint8_t mac[6];

	public:
		HostMAC()								{ memset(mac, 0, sizeof(mac)); }
		HostMAC(const HostMAC &o)				{ memcpy(mac, o.GetMAC(), sizeof(mac)); }
		explicit HostMAC(const uint8_t mac[6])	{ memcpy(this->mac, mac, sizeof(this->mac)); }
		const uint8_t *GetMAC() const			{ return mac; }
		QString ToString() const				{ return QByteArray((const char *)mac, sizeof(mac)).toHex(); }
		uint64_t GetValue() const
		{
			return ((uint64_t)mac[0] << 0x28)
				| ((uint64_t)mac[1] << 0x20)
				| ((uint64_t)mac[2] << 0x18)
				| ((uint64_t)mac[3] << 0x10)
				| ((uint64_t)mac[4] << 0x8)
				| mac[5];
		}
};

static bool operator==(const HostMAC &a, const HostMAC &b)	{ return memcmp(a.GetMAC(), b.GetMAC(), 6) == 0; }
static bool operator!=(const HostMAC &a, const HostMAC &b)	{ return !(a == b); }
static bool operator<(const HostMAC &a, const HostMAC &b)	{ return a.GetValue() < b.GetValue(); }

class HiddenHost
{
	private:
		HostMAC server_mac;
		QString server_nickname;
	public:
		HiddenHost()	{ ; }
		HiddenHost(HostMAC  server_mac, QString server_nickname)	{ this->server_mac = server_mac; this->server_nickname = server_nickname; }
		HostMAC GetMAC() const 		{ return server_mac; }
		QString GetNickname() const 	{ return server_nickname; }
		void SetNickname(const QString &nickname) { this->server_nickname = nickname; }

		void SaveToSettings(QSettings *settings) const;
		static HiddenHost LoadFromSettings(QSettings *settings);
};

static bool operator==(const HiddenHost &a, const HiddenHost &b)	{ return (a.GetMAC() == b.GetMAC() && a.GetNickname() == b.GetNickname()); }
class RegisteredHost
{
	private:
		ChiakiTarget target;
		QString ap_ssid;
		QString ap_bssid;
		QString ap_key;
		QString ap_name;
		HostMAC server_mac;
		QString server_nickname;
		char rp_regist_key[CHIAKI_SESSION_AUTH_SIZE];
		uint32_t rp_key_type;
		uint8_t rp_key[0x10];
		QString console_pin;

	public:
		RegisteredHost();
		RegisteredHost(const RegisteredHost &o);

		RegisteredHost(const ChiakiRegisteredHost &chiaki_host);
		void SetConsolePin(RegisteredHost &host, QString console_pin);
		ChiakiTarget GetTarget() const			{ return target; }
		const HostMAC &GetServerMAC() const 	{ return server_mac; }
		const QString &GetServerNickname() const	{ return server_nickname; }
		const QByteArray GetRPRegistKey() const	{ return QByteArray(rp_regist_key, sizeof(rp_regist_key)); }
		const QByteArray GetRPKey() const		{ return QByteArray((const char *)rp_key, sizeof(rp_key)); }
		const QString GetConsolePin() const				{ return console_pin; }

		void SaveToSettings(QSettings *settings) const;
		static RegisteredHost LoadFromSettings(QSettings *settings);
};

class ManualHost
{
	private:
		int id;
		QString host;
		bool registered;
		HostMAC registered_mac;

	public:
		ManualHost();
		ManualHost(int id, const QString &host, bool registered, const HostMAC &registered_mac);
		ManualHost(int id, const ManualHost &o);
		void SetHost(const QString &hostadd);

		int GetID() const 			{ return id; }
		QString GetHost() const 	{ return host; }
		bool GetRegistered() const	{ return registered; }
		HostMAC GetMAC() const 		{ return registered_mac; }

		void Register(const RegisteredHost &registered_host) { this->registered = true; this->registered_mac = registered_host.GetServerMAC(); }

		void SaveToSettings(QSettings *settings) const;
		static ManualHost LoadFromSettings(QSettings *settings);
};
static bool operator==(const ManualHost &a, const ManualHost &b)	{ return (a.GetID() == b.GetID() && a.GetHost() == b.GetHost() && a.GetRegistered() == b.GetRegistered() && a.GetMAC() == b.GetMAC()); }
class PsnHost
{
	private:
	    QString duid;
		QString name;
		bool ps5;

	public:
		PsnHost();
		PsnHost(const QString &duid, const QString &name, bool ps5);

		QString GetDuid() const          { return duid; }
		QString GetName() const          { return name; }
		bool IsPS5() const               { return ps5;  }
		ChiakiTarget GetTarget() const;

};

Q_DECLARE_METATYPE(HostMAC)
Q_DECLARE_METATYPE(RegisteredHost)
Q_DECLARE_METATYPE(ManualHost)
Q_DECLARE_METATYPE(PsnHost)

#endif //CHIAKI_HOST_H
