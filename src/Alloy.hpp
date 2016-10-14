
#ifndef _ALLOY_H_90E5361681C8_
#define _ALLOY_H_90E5361681C8_

#include "external.hpp"

struct Alloy final {
    QString m_name;

    struct Component {
        QString m_name;
        std::uint8_t m_min_percent;
        std::uint8_t m_max_percent;
    };

    QList<Component> m_components;
};

#endif /* _ALLOY_H_90E5361681C8_ */
