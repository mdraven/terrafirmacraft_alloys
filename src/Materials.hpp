
#ifndef _MATERIALS_H_BF5D08BA6195_
#define _MATERIALS_H_BF5D08BA6195_

#include "external.hpp"

struct Material;

class Materials final {
    std::map<QString, Material> m_materials;
    QPixmap m_unknown;
public:
    Materials() = default;
    Materials(Materials&&) = default;

    explicit Materials(const QString& filename);

    const QPixmap& getPixmap(const QString& name) const;

    ~Materials();
};

#endif /* _MATERIALS_H_BF5D08BA6195_ */
