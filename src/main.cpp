#include <exiv2/exiv2.hpp>
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Użycie: jpegmeta <plik.jpg>\n";
        return 1;
    }

    const std::string path = argv[1];
    try {
        auto image = Exiv2::ImageFactory::open(path);
        if (!image) {
            std::cerr << "Nie mogę otworzyć: " << path << "\n";
            return 2;
        }
        image->readMetadata();

        auto& exif = image->exifData();
        if (exif.empty()) {
            std::cout << "Brak danych EXIF w: " << path << "\n";
            return 0;
        }

        for (const auto& md : exif) {
            std::cout << md.key() << " = " << md.toString() << "\n";
        }
    } catch (const Exiv2::Error& e) {
        std::cerr << "Błąd Exiv2: " << e.what() << "\n";
        return 3;
    } catch (const std::exception& e) {
        std::cerr << "Błąd: " << e.what() << "\n";
        return 4;
    }
    return 0;
}
