#include "VpnProtocolModel.h"

// FIXME: this include should to into <openconnect.h>
#ifdef _WIN32
#include <winsock2.h>
#endif
#include <openconnect.h>

VpnProtocolModel::VpnProtocolModel(QObject* parent)
    : QAbstractListModel(parent)
{
    loadProtocols();
}

int VpnProtocolModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_protocols.size();
}

QVariant VpnProtocolModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const VpnProtocol protocol = m_protocols.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return QVariant{ protocol.prettyName };
    case Qt::ToolTipRole:
        return QVariant{ protocol.description };
    case Qt::UserRole + 1:
        return QVariant{ protocol.name };
    }

    return QVariant();
}

#if OPENCONNECT_CHECK_VER(5, 5)
void VpnProtocolModel::loadProtocols()
{
    struct oc_vpn_proto* protos = nullptr;
    int rc;

    if ((rc = openconnect_get_supported_protocols(&protos)) >= 0) {
        for (int i = 0; i != rc; ++i) {
            m_protocols.append({ protos[i].name, protos[i].pretty_name, protos[i].description });
        }
        openconnect_free_supported_protocols(protos);
    }
}
#else
void VpnProtocolModel::loadProtocols()
{
    m_protocols.append({ "anyconnect", "Cisco AnyConnect", "" });
    m_protocols.append({ "nc", "Juniper Network Connect", "" });
}
#endif
