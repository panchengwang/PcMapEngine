#include <iostream>
#include <stdlib.h>
#include "include/core/SkFontMgr.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkString.h"
#include <vector>
#include <string>
#include <iostream>
#include "include/core/SkFontScanner.h"
#include "include/ports/SkFontMgr_fontconfig.h"


/**
 * 获取字体列表
 */
std::vector<std::string> GetFontsList() {
    std::vector<std::string> fontList;
    sk_sp<SkFontMgr> fontMgr;
#if defined(SK_BUILD_FOR_UNIX)
    fontMgr = SkFontMgr_New_FontConfig(NULL, NULL);
#endif
    if (fontMgr) {
        int count = fontMgr->countFamilies();
        for (int i = 0; i < count; ++i) {
            SkString familyName;
            fontMgr->getFamilyName(i, &familyName);
            fontList.emplace_back(familyName.c_str());
        }
    }

    return fontList;
}



int main(int argc, char** argv) {

    auto fonts = GetFontsList();

    std::cout << "系统支持的字体 (" << fonts.size() << " 种):\n";
    for (const auto& font : fonts) {
        std::cout << font << "\n";
    }
    return EXIT_SUCCESS;
}