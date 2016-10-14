
#include "Alloys.hpp"

#include "Alloy.hpp"

Alloys::Alloys(const QString& filename) {
    QFile file(filename);

    file.open(QFile::ReadOnly);
    QString text = file.readAll();
    file.close();

    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(text.toUtf8(), &error);

    if(error.error != QJsonParseError::NoError) {
        qWarning() << "Error in '" << filename
                   << "': " << error.errorString();
        return;
    }

    QJsonObject root = doc.object();

    for(auto root_it = root.begin(); root_it != root.end(); ++root_it) {
        QString alloy_name = root_it.key();
        QJsonValue comp = root_it.value();

        if(!comp.isObject()) {
            qWarning() << "Error in '" << filename << "': "
                       << "alloy '" << alloy_name
                       << "' has incorrect component's properties";
            m_alloys.clear();
            return;
        }

        QJsonObject comp_obj = comp.toObject();
        QList<Alloy::Component> components;

        for(auto comp_it = comp_obj.begin(); comp_it != comp_obj.end(); ++comp_it) {
            QString component_name = comp_it.key();
            QJsonValue prop = comp_it.value();

            if(!prop.isObject()) {
                qWarning() << "Error in '" << filename << "': "
                           << "alloy '" << alloy_name
                           << "' component '" << component_name
                           << "' has incorrect properties";
                m_alloys.clear();
                return;
            }

            QJsonObject prop_obj = prop.toObject();
            QJsonValue min_percent_val = prop_obj.value("min_percent");
            QJsonValue max_percent_val = prop_obj.value("max_percent");

            if(!min_percent_val.isDouble() || !max_percent_val.isDouble()) {
                qWarning() << "Error in '" << filename << "': "
                           << "alloy '" << alloy_name
                           << "' component '" << component_name
                           << "' has incorrect '(max/min)_percent' values";
                m_alloys.clear();
                return;
            }

            std::uint8_t min_percent = min_percent_val.toInt();
            std::uint8_t max_percent = max_percent_val.toInt();
            if(min_percent >= max_percent) {
                qWarning() << "Error in '" << filename << "': "
                           << "alloy '" << alloy_name
                           << "' component '" << component_name
                           << "' has incorrect '(max/min)_percent' values";
                m_alloys.clear();
                return;
            }

            Alloy::Component component{component_name, min_percent, max_percent};
            components.push_back(component);
        }

        m_alloys[alloy_name] = Alloy{alloy_name, components};
    }
}

const Alloy* Alloys::get(const QString& name) const {
    auto it = m_alloys.find(name);
    if(it == m_alloys.end())
        return nullptr;
    return &it.value();
}

void Alloys::getAlloysName(QList<QString>& names) const {
    names.clear();

    for(auto it = m_alloys.keyBegin(); it != m_alloys.keyEnd(); ++it)
        names.push_back(*it);
}

Alloys::~Alloys() {}
