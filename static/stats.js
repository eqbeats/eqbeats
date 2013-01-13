//"use strict";

clone = function(obj) { // http://my.opera.com/GreyWyvern/blog/show.dml/1725165
    var newObj = (obj instanceof Array) ? [] : {};
    for (i in obj) {
        if (i == 'clone') continue;
        if (obj[i] && typeof obj[i] == "object") {
            newObj[i] = clone(obj[i]);
        } else newObj[i] = obj[i]
    } return newObj;
};
var daysecs = 60*60*24;
timescale = d3.scale.linear().domain([today() - daysecs * 60, today() + daysecs ]).range([0, 650]);
days = 30;
var data = [];
uniqmode = true;
statsfirstrun = true;

charts = [
    { "name": "Views", "id": "views", "classname": "views", "type": "trackView", "height":150, "width": 650, "data": [], "uniqdata": [] },
    { "name": "Plays", "id": "plays", "classname": "plays", "type": "trackPlay", "height":100, "width": 650, "data": [], "uniqdata": [] },
    { "name": "Downloads", "id": "dls", "classname": "dls", "type": "trackDownload", "height":100, "width": 650, "data": [], "uniqdata": [] }
]

timeranges = [
    { "name": "A month", "days": "30" },
    { "name": "Two months", "days": "61" },
    { "name": "Four months", "days": "122" },
    { "name": "Six months", "days": "183" },
    { "name": "A year", "days": "365" }
]


function now(){ return Math.floor(+(new Date())/1000); }
function day(t){ return t - (t % 86400); }
function today(){ return day(now()); }

function log(l, n){ return Math.log(n)/Math.log(l); }

function initstats(){
    if(statsfirstrun){
        statsfirstrun = false;
        var chartdiv = d3.select("div#charts").style("display", "block");
        window.location = "#charts";
        chartdiv.append("h3").html("<img src='/static/icons/system-monitor-24.png' alt=''/>Statistics");
        var inner = chartdiv.append("div").classed("double-column", true);
        var settings = inner.append("div").classed("settings", true);
        var span = settings.append("span");
        span.append("input").attr("id", "unique").attr("type", "checkbox")
            .property("checked", uniqmode)
            .on("change", function(){
                uniqmode = this.checked;
                render();
            });
        span.append("label").text("Unique only").attr("for", "unique");
        var span = settings.append("span");
        var select = span.append("select").attr("id", "range")
        select.selectAll("option")
            .data(timeranges).enter()
            .append("option")
            .attr("value", function(d){return d.days;})
            .text(function(d){return d.name;});
        select.property("value", days)
            .on("change", function(){
                days = this.value;
                render();
            });

        inner.selectAll("svg.chart").data(charts).enter()
            .append("svg")
            .classed("chart", true)
            .attr("width", function(d){return d.width;})
            .attr("height", function(d){return d.height;})
            .attr("id", function(d){return d.id;})
            .append("text")
            .classed("chartlabel", true)
            .attr("y", 15)
            .attr("x", 5)
            .text(function(d){return d.name;});

        inner.append("h4").html("<img src='/static/icons/chain--arrow.png' alt=''/> Referrers");
        inner.append("table").classed("referrers", true);
    }
    var ellipsis = d3.select("#charts").append("div").classed("ellipsis", true);
    window.location = "#charts";
    var xhr = new XMLHttpRequest();
    xhr.open("GET", window.location.protocol + "//" + window.location.host + (window.location.port != ''? ":" + window.location.port : '') + window.location.pathname + "/stats", true);
    //xhr.open("GET", "https://eqbeats.org/track/660/stats", true);
    xhr.onreadystatechange = function(){
        if(xhr.readyState == 4){
            data = eval(xhr.responseText);
            render(true);
            d3.select(".ellipsis").remove();
        }
    }
    xhr.send();
}


function render(refilter){
    if(refilter){
        var cf = crossfilter(data);
        var entriesByTime = cf.dimension(function(d){return d.timestamp;});
        var daily = entriesByTime.group(function(t){ return t - (t % daysecs); });
        var entriesByType = cf.dimension(function(d){return d.type;});
        for(var i = 0; i < charts.length; i++){
            entriesByType.filter(charts[i].type);
            charts[i].data = clone(daily.all());
        }
        entriesByType.filter("trackView");
        var entriesByReferrer = cf.dimension(function(d){return d.referrer || '';});
        var entriesGroupByReferringDomain = entriesByReferrer.group(function(ref){return ref.replace(/^https?:\/\/(www\.)?([^\/]*).*/, "$2");});
        referrers = clone(entriesGroupByReferringDomain.top(6));
        cf = crossfilter(uniq(data));
        entriesByTime = cf.dimension(function(d){return d.timestamp;});
        daily = entriesByTime.group(function(t){ return t - (t % daysecs); });
        entriesByType = cf.dimension(function(d){return d.type;});
    for(var i = 0; i < charts.length; i++){
            entriesByType.filter(charts[i].type);
            charts[i].uniqdata = clone(daily.all());
        }
    }
    for(var i = 0; i < charts.length; i++){
        var chart = charts[i];

        // grab in-range days only
        var chartdata_ = uniqmode?chart.uniqdata:chart.data;
        var chartdata = Array();
        for(var j = 0; j < chartdata_.length; j++){
            if(chartdata_[j].key >= today() - (days - 1) * daysecs)
                chartdata.push(chartdata_[j]);
        }

        // scale
        timescale = timescale.domain([today() - daysecs * (days - 1), today() + daysecs]);
        var ceiling = 20;
        for(var j = 0; j < chartdata.length; j++)
            while(chartdata[j].value >= ceiling - 1){
                var power = Math.pow(10, Math.floor(log(10, ceiling)));
                if(Math.round(ceiling / power) < 2){ ceiling = 2 * power; }
                else if(Math.round(ceiling / power) < 4){ ceiling = 4 * power; }
                else if(Math.round(ceiling / power) < 6){ ceiling = 6 * power; }
                else{ ceiling = 10 * power; }
            }
        var yscale = d3.scale.linear().domain([0, ceiling]).range([0, chart.height]).nice();
        var prevyscale = chart.yscale || yscale;
        chart.yscale = yscale;

        // making bars
        var update = d3.select("#" + chart.id).selectAll("." + chart.classname)
            .data(chartdata, function(d){ return d.key; });
        update.enter()
            .append("rect")
            .attr("height", 1).attr("y", chart.height)
            .attr("width", timescale(daysecs) - timescale(0) + 2)
            .attr("x", function(d){ return Math.floor(timescale(d.key) - 1); })
            .classed(chart.classname, true).classed("bar", true);
        update.transition()
            .attr("height", function(d){ return yscale(d.value); })
            .attr("y", function(d){ return chart.height - yscale(d.value); })
            .attr("width", Math.floor(timescale(daysecs) - timescale(0)) + 2)
            .attr("x", function(d){ return Math.floor(timescale(d.key) - 1); });
        update.exit().transition()
            .attr("height", 0)
            .attr("y", chart.height)
            .attr("width", Math.floor(timescale(daysecs) - timescale(0)) + 2)
            .attr("x", function(d){ return Math.floor(timescale(d.key) - 1); });

        // ticks
        var ticks = yscale.ticks(3).slice(1, -1);
        var update = d3.select("#" + chart.id).selectAll(".ytick").data(ticks, function(d){return d});
        update.enter().append("rect")
            .classed("tick", true).classed("ytick", true)
            .attr("x", 0)
            .attr("width", chart.width)
            .attr("height", 1)
            .attr("y", function(d){ return chart.height - prevyscale(d); })
            .style("opacity", 0);
        update.transition()
            .attr("y", function(d){ return chart.height - yscale(d); })
            .style("opacity", 0.5);
        update.exit().transition()
            .attr("y", function(d){ return chart.height - yscale(d); })
            .style("opacity", 0.2)
            .transition().style("opacity", 0).remove();
        var labels = d3.select("#" + chart.id).selectAll(".ylabel").data(ticks, function(d){return d;});
        // tick labels
        labels.enter().append("text")
            .classed("ylabel", true)
            .attr("x", 5)
            .attr("y", function(d){ return chart.height - prevyscale(d) - 5; })
            .text(function(d){return d;})
            .style("opacity", 0);
        labels.transition()
            .attr("y", function(d){ return chart.height - yscale(d) - 5; })
            .style("opacity", 0.5);
        labels.exit().transition()
            .attr("y", function(d){ return chart.height - yscale(d) - 5; })
            .style("opacity", 0.2)
            .transition().style("opacity", 0).remove();

    }

    // referrers
    var update = d3.select(".referrers").selectAll("tr").data(referrers);
    var tr = update.enter().append("tr");
    tr.append("td").html(function(d){ return d.key==""?"No address (IM, Email, Bookmarks...)</td>":"<a href=\"http://" + d.key + "/\">" + d.key + "</a>"; });
    tr.append("td").classed("value", true).html(function(d){ return d.value; });
    update.exit().remove();

}

function uniq(data){
    out = Array();
    seen = Array();
    for(var i=0; i < data.length; i++){
        row = data[i];
        switch(row.unique){
            case 1:
                out.push(row);
                break;
            case -1:
            var hash = row.type + row.addr + day(row.timestamp);
            if(seen.indexOf(hash) == -1){
                seen.push(hash);
                out.push(row);
            }
        }
    }
    return out;
}
