#ifndef TEXT_MODIFIERS
#define TEXT_MODIFIERS

#include <core/template.h>

class IrcEscape: public ctemplate::TemplateModifier {
    void Modify(const char*, size_t, const ctemplate::PerExpandData*, ctemplate::ExpandEmitter*, const std::string&) const;
};

class Formatter: public ctemplate::TemplateModifier {
    void Modify(const char*, size_t, const ctemplate::PerExpandData*, ctemplate::ExpandEmitter*, const std::string&) const;
};

#endif // TEXT_MODIFIERS
