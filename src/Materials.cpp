
#include "Materials.hpp"

#include "Material.hpp"

static const QString g_unknown_material = ":/unknown";
static const QString g_image_directory = "images/";

Materials::Materials(const QString& filename) {
    if(m_unknown.load(g_unknown_material) == false) {
        qWarning() << "Cannot load pixmap for undefined image";
        return;
    }

    QFile file(filename);

    file.open(QFile::ReadOnly);
    QString text = file.readAll();
    file.close();

    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(text.toUtf8(), &error);

    if(error.error != QJsonParseError::NoError) {
        qWarning() << "Error in '" << filename << "': " << error.errorString();
        return;
    }

    QJsonObject root = doc.object();

    for(auto it = root.begin(); it != root.end(); ++it) {
        QString name = it.key();
        QJsonValue prop = it.value();

        if(!prop.isObject()) {
            qWarning() << "Error in '" << filename << "': "
                       << "material '" << name << "' has incorrect properties";
            m_materials.clear();
            return;
        }

        QJsonObject obj = prop.toObject();
        QJsonValue image = obj.value("image");

        QString image_file(g_unknown_material);
        if(image.isString())
            image_file = g_image_directory + image.toString();

        QPixmap pixmap;
        if(pixmap.load(image_file) == false) {
            qWarning() << "Error in '" << filename << "': "
                       << "cannot load image '" << image_file;
            pixmap = m_unknown;
        }

        m_materials[name] = Material{name, pixmap};
    }
}

const QPixmap& Materials::getPixmap(const QString& name) const {
    auto it = m_materials.find(name);
    if(it == m_materials.end())
        return m_unknown;
    return it->second.m_image;
}

Materials::~Materials() {}
