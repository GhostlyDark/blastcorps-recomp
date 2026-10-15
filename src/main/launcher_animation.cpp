#include "banjo_launcher.h"
#include "elements/ui_image.h"
#include "elements/ui_label.h"
#include "util/file.h"
#include <atomic>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

#include "../../lib/rt64/src/contrib/stb/stb_image.h"

namespace {
    constexpr const char* splash_asset_name = "splash_by_jmanvelez.png";
    constexpr const char* splash_src = "?/blastcorps_launcher/splash";

    std::vector<char> read_asset_bytes(const char* asset_name) {
        std::filesystem::path path = recompui::file::get_asset_path(asset_name);
        std::ifstream file{ path, std::ios::binary };
        if (!file) {
            return {};
        }

        return { std::istreambuf_iterator<char>{ file }, std::istreambuf_iterator<char>{} };
    }

    bool queue_splash_image() {
        std::vector<char> png_bytes = read_asset_bytes(splash_asset_name);
        if (png_bytes.empty()) {
            return false;
        }

        int width = 0;
        int height = 0;
        stbi_uc* rgba = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(png_bytes.data()),
                                              static_cast<int>(png_bytes.size()), &width, &height, nullptr, 4);

        bool queued = false;
        if (rgba != nullptr && width > 0 && height > 0) {
            std::vector<char> rgba_bytes{ reinterpret_cast<char*>(rgba),
                                          reinterpret_cast<char*>(rgba) + (width * height * 4) };
            recompui::release_image(splash_src);
            recompui::queue_image_from_bytes_rgba32(splash_src, rgba_bytes, width, height);
            queued = true;
        }

        if (rgba != nullptr) {
            stbi_image_free(rgba);
        }

        return queued;
    }
}

void banjo::launcher_animation_setup(recompui::LauncherMenu *menu) {
    auto context = recompui::get_current_context();
    recompui::Element *background_container = menu->get_background_container();
    background_container->set_background_color({ 0x46, 0x46, 0x46, 0xFF });

    if (queue_splash_image()) {
        recompui::Image *splash_image = context.create_element<recompui::Image>(background_container, splash_src);
        splash_image->set_position(recompui::Position::Absolute);
        splash_image->set_top(0);
        splash_image->set_left(0);
        splash_image->set_height(100.0f, recompui::Unit::Percent);
        splash_image->set_width_auto();
    }

    menu->remove_default_title();

    recompui::Label* title_label = context.create_element<recompui::Label>(background_container, "BLASTCORPS",
                                                                           recompui::theme::Typography::Header1);
    title_label->set_position(recompui::Position::Absolute);
    title_label->set_top(200.0f);
    title_label->set_right(120.0f);
    title_label->set_font_family("CutOutsFLF");
    title_label->set_font_size(140.0f);
    title_label->set_color({ 0xFF, 0xFF, 0xFF, 0xFF });


    recompui::Label* subtitle_label = context.create_element<recompui::Label>(background_container, "RECOMPILED",
                                                                           recompui::theme::Typography::Header2);
    subtitle_label->set_position(recompui::Position::Absolute);
    subtitle_label->set_top(300.0f);
    subtitle_label->set_right(120.0f);
    subtitle_label->set_font_family("CutOutsFLF");
    subtitle_label->set_font_size(72.0f);
    subtitle_label->set_color({ 0xFF, 0xFF, 0xFF, 0xFF });
}

void banjo::launcher_animation_update(recompui::LauncherMenu *menu) {
}
