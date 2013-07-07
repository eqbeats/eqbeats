{
    {{#HAS_UNIQUE}}

    "unique_totals": {
        {{#UNIQUE_MEASUREMENT}}
        "{{MEASUREMENT:o}}": {{VALUE}}{{#UNIQUE_MEASUREMENT_separator}}, {{/UNIQUE_MEASUREMENT_separator}}
        {{/UNIQUE_MEASUREMENT}}
    },

    {{#HAS_DAILY}}
    "unique_days": {
        {{#DAILY_UNIQUE_MEASUREMENT}}
        "{{MEASUREMENT:o}}": {
            {{#DAY}}
            "{{DAY:o}}": {{VALUE}}{{#DAY_separator}}, {{/DAY_separator}}
            {{/DAY}}
        }
        {{#DAILY_UNIQUE_MEASUREMENT_separator}}, {{/DAILY_UNIQUE_MEASUREMENT_separator}}
        {{/DAILY_UNIQUE_MEASUREMENT}}
    },
    {{/HAS_DAILY}}

    {{/HAS_UNIQUE}}

    "totals": {
        {{#MEASUREMENT}}
        "{{MEASUREMENT:o}}": {{VALUE}}{{#MEASUREMENT_separator}}, {{/MEASUREMENT_separator}}
        {{/MEASUREMENT}}
    }
    {{#HAS_DAILY}},
    "days": {
        {{#DAILY_MEASUREMENT}}
        "{{MEASUREMENT:o}}": {
            {{#DAY}}
            "{{DAY:o}}": {{VALUE}}{{#DAY_separator}}, {{/DAY_separator}}
            {{/DAY}}
        }
        {{#DAILY_MEASUREMENT_separator}}, {{/DAILY_MEASUREMENT_separator}}
        {{/DAILY_MEASUREMENT}}
    }
    {{/HAS_DAILY}}
    {{#HAS_REFERRERS}},
    "referrers": {
        {{#REFERRER}}
            "{{REFERRER:o}}": {{VALUE}}{{#REFERRER_separator}}, {{/REFERRER_separator}}
        {{/REFERRER}}
    }
    {{/HAS_REFERRERS}}
}
