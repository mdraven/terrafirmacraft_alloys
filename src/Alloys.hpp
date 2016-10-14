
#ifndef _ALLOYS_H_3602E319DFFB_
#define _ALLOYS_H_3602E319DFFB_

#include "external.hpp"

struct Alloy;

class Alloys final {
    QMap<QString, Alloy> m_alloys;
public:
    Alloys() = default;
    Alloys(Alloys&&) = default;

    explicit Alloys(const QString& filename);

    const Alloy* get(const QString& name) const;

    void getAlloysName(QList<QString>& names) const;

    ~Alloys();
};

#endif /* _ALLOYS_H_3602E319DFFB_ */
