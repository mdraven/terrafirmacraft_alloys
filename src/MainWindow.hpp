
#ifndef _MAINWINDOW_H_BE1330D8C04E_
#define _MAINWINDOW_H_BE1330D8C04E_

#include "external.hpp"

class Alloys;
class Materials;

class MainWindow final : public QObject {
    Q_OBJECT;

    QMainWindow* m_main_window{};
    QLabel* m_alloy_image{};
    QComboBox* m_alloy{};
    QLabel* m_total_units{};
    QTableWidget* m_components{};

    const Alloys* m_alloys{};
    const Materials* m_materials{};

    bool loadUi();
    void configComponentsTable();
    void updateAlloyComboBox();
    void updateComponentsTable();
    void setComponentsTableRow(int row, const QString& comp_name,
                               std::uint8_t min_percent,
                               std::uint8_t max_percent);
    void updateCurPercentCells();
    void colorCurPercentCells();
private Q_SLOTS:
    void changedAlloy(const QString& alloy_name);
    void expressionChanged(int row, int column);
public:
    MainWindow();

    void setMaterials(const Materials* materials);
    void setAlloys(const Alloys* alloys);

    ~MainWindow();
public Q_SLOTS:
    void show();
    void hide();
};

#endif /* _MAINWINDOW_H_BE1330D8C04E_ */
