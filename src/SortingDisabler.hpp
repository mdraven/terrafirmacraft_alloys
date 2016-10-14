
#ifndef _SORTINGDISABLER_H_F3192560FFC7_
#define _SORTINGDISABLER_H_F3192560FFC7_

class QTableView;

class SortingDisabler final {
    bool m_enabled;
    QTableView* m_table_view;
public:
    SortingDisabler(QTableView* table_view)
        : m_table_view(table_view) {
        if(m_table_view == nullptr)
            return;

        m_enabled = m_table_view->isSortingEnabled();

        if(m_enabled)
            m_table_view->setSortingEnabled(false);
    }

    // forbid copying
    SortingDisabler(const SortingDisabler&) = delete;
    SortingDisabler& operator=(const SortingDisabler&) = delete;

    ~SortingDisabler() {
        if(m_table_view && m_enabled)
            m_table_view->setSortingEnabled(true);
    }
};

#endif /* _SORTINGDISABLER_H_F3192560FFC7_ */
