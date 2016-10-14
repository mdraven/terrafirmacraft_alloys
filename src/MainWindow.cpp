
#include "MainWindow.hpp"

#include "Alloy.hpp"
#include "Alloys.hpp"
#include "Calculator.hpp"
#include "Materials.hpp"
#include "SortingDisabler.hpp"

static const QString g_main_window = ":/main_window";
static const QString g_window_title = "Alloys calculator";
static const QString g_total_units = "Total units: ";

enum Col { name, expression, value, min_percent, cur_percent, max_percent };

MainWindow::MainWindow() {
    if(!loadUi())
        return;

    configComponentsTable();

    connect(m_alloy, static_cast<void (QComboBox::*)(const QString&)>(
                         &QComboBox::currentIndexChanged),
            this, &MainWindow::changedAlloy);
}

void MainWindow::configComponentsTable() {
    QStringList header_labels{"Name", "Expression", "Value",
                              "Min%", "Cur%",       "Max%"};
    m_components->setColumnCount(header_labels.size());
    m_components->setHorizontalHeaderLabels(header_labels);

    m_components->setColumnWidth(Col::name, 90);
    m_components->setColumnWidth(Col::expression, 100);
    m_components->setColumnWidth(Col::value, 45);
    m_components->setColumnWidth(Col::min_percent, 35);
    m_components->setColumnWidth(Col::cur_percent, 40);
    m_components->setColumnWidth(Col::max_percent, 35);

    QHeaderView* vertical_header = m_components->verticalHeader();
    vertical_header->setDefaultSectionSize(20);

    connect(m_components, &QTableWidget::cellChanged, this,
            &MainWindow::expressionChanged);
}

bool MainWindow::loadUi() {
    QUiLoader loader;
    QFile file(g_main_window);

    file.open(QFile::ReadOnly);
    m_main_window = qobject_cast<QMainWindow*>(loader.load(&file));
    file.close();

    if(m_main_window == nullptr) {
        qWarning() << "Cannot load 'main_window'";
        return false;
    }

    bool ok_loading = true;

    m_alloy_image = m_main_window->findChild<QLabel*>("alloy_image");
    if(m_alloy_image == nullptr) {
        qWarning() << "Cannot load 'alloy_image'";
        ok_loading = false;
    }

    m_alloy = m_main_window->findChild<QComboBox*>("alloy");
    if(m_alloy == nullptr) {
        qWarning() << "Cannot load 'alloy'";
        ok_loading = false;
    }

    m_total_units = m_main_window->findChild<QLabel*>("total_units");
    if(m_total_units == nullptr) {
        qWarning() << "Cannot load 'total_units'";
        ok_loading = false;
    }

    m_components = m_main_window->findChild<QTableWidget*>("components");
    if(m_components == nullptr) {
        qWarning() << "Cannot load 'components'";
        ok_loading = false;
    }

    if(ok_loading == false) {
        m_main_window = nullptr;
        m_alloy_image = nullptr;
        m_alloy = nullptr;
        m_total_units = nullptr;
        m_components = nullptr;
        return false;
    }

    return true;
}

void MainWindow::expressionChanged(int row, int column) {
    if(column != Col::expression)
        return;

    QTableWidgetItem* expr_cell = m_components->item(row, Col::expression);
    if(expr_cell == nullptr) {
        qWarning() << "Cell 'expression' is null";
        return;
    }

    const QString& text = expr_cell->text();

    QTableWidgetItem* value_cell = m_components->item(row, Col::value);
    if(value_cell == nullptr) {
        qWarning() << "Cell 'value' is null";
        return;
    }

    Calculator calc(text.toStdString());
    auto calc_res = calc.get();
    if(calc_res)
        value_cell->setText(QString::number(*calc_res));
    else
        value_cell->setText(calc.status());

    updateCurPercentCells();
    colorCurPercentCells();
}

void MainWindow::colorCurPercentCells() {
    for(int i = 0; i < m_components->rowCount(); ++i) {
        QTableWidgetItem* min_percent_cell =
            m_components->item(i, Col::min_percent);
        if(min_percent_cell == nullptr) {
            qWarning() << "Cell 'min_percent' is null";
            return;
        }

        QTableWidgetItem* cur_percent_cell =
            m_components->item(i, Col::cur_percent);
        if(cur_percent_cell == nullptr) {
            qWarning() << "Cell 'cur_percent' is null";
            return;
        }

        QTableWidgetItem* max_percent_cell =
            m_components->item(i, Col::max_percent);
        if(max_percent_cell == nullptr) {
            qWarning() << "Cell 'max_percent' is null";
            return;
        }

        const QString& text = cur_percent_cell->text();
        bool ok;
        double cur_percent = text.toDouble(&ok);
        if(!ok) {
            cur_percent_cell->setForeground(QBrush(Qt::red));
            continue;
        }

        double min_percent = min_percent_cell->text().toDouble();
        double max_percent = max_percent_cell->text().toDouble();
        if(cur_percent < min_percent || cur_percent > max_percent)
            cur_percent_cell->setForeground(QBrush(Qt::red));
        else
            cur_percent_cell->setForeground(QBrush(Qt::green));
    }
}

void MainWindow::updateCurPercentCells() {
    double sum = 0.0;

    for(int i = 0; i < m_components->rowCount(); ++i) {
        QTableWidgetItem* value_cell = m_components->item(i, Col::value);
        if(value_cell == nullptr) {
            qWarning() << "Cell 'value' is null";
            return;
        }

        const QString& text = value_cell->text();
        bool ok;
        double val = text.toDouble(&ok);
        if(!ok) {
            sum = 0.0;
            break;
        }
        sum += val;
    }

    const SortingDisabler disabler(m_components);

    for(int i = 0; i < m_components->rowCount(); ++i) {
        QTableWidgetItem* cur_percent_cell =
            m_components->item(i, Col::cur_percent);
        if(cur_percent_cell == nullptr) {
            qWarning() << "Cell 'cur_percent' is null";
            return;
        }

        if(sum < std::numeric_limits<double>::epsilon())
            cur_percent_cell->setText("Error");
        else {
            QTableWidgetItem* value_cell = m_components->item(i, Col::value);
            if(value_cell == nullptr) {
                qWarning() << "Cell 'value' is null";
                return;
            }

            const QString& text = value_cell->text();
            double val = text.toDouble();

            double cur_percent = (val * 100.0) / sum;
            cur_percent = std::round(cur_percent * 10.0) / 10.0;

            cur_percent_cell->setText(QString::number(cur_percent));
        }
    }

    m_total_units->setText(g_total_units + QString::number(sum));
}

void MainWindow::changedAlloy(const QString& alloy_name) {
    if(m_materials) {
        const QPixmap& pixmap = m_materials->getPixmap(alloy_name);
        m_alloy_image->setPixmap(pixmap);
        m_main_window->setWindowIcon(pixmap);
    }

    m_main_window->setWindowTitle(alloy_name + " - " + g_window_title);

    updateComponentsTable();
}

void MainWindow::setComponentsTableRow(int row, const QString& comp_name,
                                       std::uint8_t min_percent,
                                       std::uint8_t max_percent) {
    QTableWidgetItem* name_cell = nullptr;

    if(m_materials)
        name_cell =
            new QTableWidgetItem(m_materials->getPixmap(comp_name), comp_name);
    else
        name_cell = new QTableWidgetItem(comp_name);

    name_cell->setFlags(name_cell->flags() ^ Qt::ItemIsEditable);
    m_components->setItem(row, Col::name, name_cell);

    QTableWidgetItem* expr_cell = new QTableWidgetItem();
    m_components->setItem(row, Col::expression, expr_cell);

    QTableWidgetItem* value_cell = new QTableWidgetItem();
    value_cell->setFlags(value_cell->flags() ^ Qt::ItemIsEditable);
    value_cell->setTextAlignment(Qt::AlignRight);
    m_components->setItem(row, Col::value, value_cell);

    QTableWidgetItem* min_percent_cell =
        new QTableWidgetItem(QString::number(min_percent));
    min_percent_cell->setFlags(min_percent_cell->flags() ^ Qt::ItemIsEditable);
    min_percent_cell->setTextAlignment(Qt::AlignRight);
    m_components->setItem(row, Col::min_percent, min_percent_cell);

    QTableWidgetItem* cur_percent_cell = new QTableWidgetItem();
    cur_percent_cell->setFlags(cur_percent_cell->flags() ^ Qt::ItemIsEditable);
    cur_percent_cell->setTextAlignment(Qt::AlignRight);
    m_components->setItem(row, Col::cur_percent, cur_percent_cell);

    QTableWidgetItem* max_percent_cell =
        new QTableWidgetItem(QString::number(max_percent));
    max_percent_cell->setFlags(max_percent_cell->flags() ^ Qt::ItemIsEditable);
    max_percent_cell->setTextAlignment(Qt::AlignRight);
    m_components->setItem(row, Col::max_percent, max_percent_cell);
}

void MainWindow::updateComponentsTable() {
    const QString& alloy_name = m_alloy->currentText();

    m_components->clearContents();

    const Alloy* alloy = m_alloys->get(alloy_name);
    if(alloy == nullptr)
        return;

    m_components->setRowCount(alloy->m_components.size());

    const QSignalBlocker blocker(m_components);
    const SortingDisabler disabler(m_components);

    for(int i = 0; i < alloy->m_components.size(); ++i) {
        const Alloy::Component& comp = alloy->m_components[i];
        setComponentsTableRow(i, comp.m_name, comp.m_min_percent,
                              comp.m_max_percent);
    }
}

void MainWindow::updateAlloyComboBox() {
    m_alloy->clear();

    if(!m_alloys)
        return;

    QList<QString> names;
    m_alloys->getAlloysName(names);

    for(const auto& name : names) {
        if(m_materials)
            m_alloy->addItem(m_materials->getPixmap(name), name);
        else
            m_alloy->addItem(name);
    }
}

void MainWindow::setMaterials(const Materials* materials) {
    m_materials = materials;
    updateAlloyComboBox();
}

void MainWindow::setAlloys(const Alloys* alloys) {
    m_alloys = alloys;
    updateAlloyComboBox();
}

void MainWindow::show() {
    m_main_window->show();
}

void MainWindow::hide() {
    m_main_window->hide();
}

MainWindow::~MainWindow() {}
