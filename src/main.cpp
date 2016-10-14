
#include "MainWindow.hpp"

#include "Alloys.hpp"
#include "Materials.hpp"

#include "external.hpp"

int main(int argc, char *argv[]) {
    try {
        // Calculator calc1("(1.1+2) * 3.14");
        // auto calc_res = calc1.get();
        // if(calc_res)
        //     qWarning() << *calc_res;
        // else
        //     qWarning() << calc1.status();

        QApplication app(argc, argv);

        Alloys alloys("data/alloys.json");
        Materials materials("data/materials.json");

        MainWindow main_window;
        main_window.setMaterials(&materials);
        main_window.setAlloys(&alloys);

        main_window.show();

        app.exec();
    }
    catch(const std::exception& e) {
        std::cout << "Exception was catched in main(): " << e.what() << std::endl;
        throw;
    }

    return 0;
}
