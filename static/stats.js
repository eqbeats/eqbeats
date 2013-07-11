"use strict";

var clone = function(obj) { // http://my.opera.com/GreyWyvern/blog/show.dml/1725165
    var newObj = (obj instanceof Array) ? [] : {};
    for (var i in obj) {
        if (i == 'clone') continue;
        if (obj[i] && typeof obj[i] == "object") {
            newObj[i] = clone(obj[i]);
        } else newObj[i] = obj[i]
    } return newObj;
};
var daysecs = 60*60*24;
var timescale = d3.scale.linear().domain([today() - daysecs * 60, today() + daysecs ]).range([0, 650]);
var timerange = 30;
var data = {};
var uniqmode = true;
var statsfirstrun = true;

var charts = [
    { "name": "Views", "id": "views", "classname": "views", "type": "trackView", "height":150, "width": 650, "data": [], "uniqdata": [] },
    { "name": "Plays", "id": "plays", "classname": "plays", "type": "trackPlay", "height":100, "width": 650, "data": [], "uniqdata": [] },
    { "name": "Downloads", "id": "dls", "classname": "dls", "type": "trackDownload", "height":100, "width": 650, "data": [], "uniqdata": [] }
]

var timeranges = [
    { "name": "A month", "days": "30" },
    { "name": "Two months", "days": "61" },
    { "name": "Four months", "days": "122" },
    { "name": "Six months", "days": "183" },
    { "name": "A year", "days": "365" }
]


function ts(datestring){ return (new Date(datestring) - 0)/1000; }
function now(){ return Math.floor(+(new Date())/1000); }
function day(t){ return t - (t % 86400); }
function today(){ return day(now()); }

function log(l, n){ return Math.log(n)/Math.log(l); }

function initstats(){
    if(statsfirstrun){
        statsfirstrun = false;
        var chartdiv = d3.select("div#charts").style("display", "block");
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
        select.property("value", timerange)
            .on("change", function(){
                timerange = this.value;
                render();
            });

        var ins = inner.append("div").classed("inspector", true);
        ins.append("span").classed("date", true).html("&nbsp;");
        for(var i = 0; i < charts.length; i++)
            ins.append("span").classed(charts[i].classname, true);

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
        var table = inner.append("table");
        table.classed("referrers", true);
        var colgroup = table.append("colgroup");
        colgroup.append("col");
        colgroup.append("col").classed("value", true);
    }
    var ellipsis = d3.select("#charts").append("div").classed("ellipsis", true);
    var xhr = new XMLHttpRequest();
    xhr.open("GET", window.location.pathname + "/stats?" + now(), true);
    xhr.onreadystatechange = function(){
        if(xhr.readyState == 4){
            data = eval(xhr.responseText)[0];
            render(true);
            d3.select(".ellipsis").remove();
        }
    }
    xhr.send();
}

var dateformat = d3.time.format("%a, %b %e");
function barover(d){
    var thisd = d;
    d3.selectAll("#charts .bar").style("opacity", 1);
    var bars = d3.selectAll("#charts .bar").filter(function(d){return d.key == thisd.key;})
        .style("opacity", 0.5);
    var ins = d3.select("#charts .inspector");
    ins.select(".date").text(dateformat(new Date(d.key)) + ": ");
    for(var i = 0; i < charts.length; i++){
        var chart = charts[i];
        ins.select("." + chart.classname).text(function(){
            var days = data.days[chart.type];
            var unique_days = data.unique_days[chart.type];
            if(days[d.key])
                    return chart.name + ": " + unique_days[d.key] + " (" + days[d.key] + " total) ";
            return "";
        });
    }
}

var referring_domains;

function quicksort(array, val_f){
    if(array.length == 0) return array;
    var lower = []; var higher = [];
    for(var i = 1; i < array.length; i++){
        if(val_f(array[i]) > val_f(array[0]))
            higher.push(array[i]);
        else
            lower.push(array[i]);
    }
    return quicksort(higher, val_f).concat( [array[0]].concat(quicksort(lower, val_f)) );
}

function exists(array, value){
    for(var i = 0; i < array.length; i++)
        if(value == array[i])
            return true;
    return false;
}

function obj2arr(object){
    var array = [];
    for(var key in object)
        array.push({"key": key, "value": object[key]});
    return array;
}

function render(refilter){
    if(refilter){
        referring_domains = [];
        var seen = [];
        for(var referrer in data.referrers){
            var domain = referrer.replace(/^https?:\/\/(www\.)?([^\/]*).*$/, "$2");
            if(!exists(seen, domain)){
                referring_domains.push({"domain": domain, "hits":data.referrers[referrer], "urls": [{"url":referrer, "hits":data.referrers[referrer]}]});
                seen.push(domain);
            }
            else{
                for(var i = 0; i < referring_domains.length; i++)
                    if(referring_domains[i].domain == domain){
                        referring_domains[i].hits += data.referrers[referrer];
                        referring_domains[i].urls.push({"url":referrer, "hits":data.referrers[referrer]});
                    }
            }
        }
        referring_domains = quicksort(referring_domains, function(d){ return d.hits }).slice(0, 6);
        // the urls should already be sorted, since they are sorted in the JSON
    }
    for(var i = 0; i < charts.length; i++){
        var chart = charts[i];

        // grab in-range days only
        var start = new Date();
        start.setDate(start.getDate() - (timerange - 1));
        var days_ = uniqmode?data.unique_days[chart.type]:data.days[chart.type];
        var days = {};
        for(var day in days_){
            if(new Date(day) >= start)
                days[day] = days_[day];
        }

        // scale
        timescale = timescale.domain([ts(start), today() + daysecs]);
        var ceiling = 20;
        for(var day in days)
            while(days[day] >= ceiling - 1){
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
            .data(obj2arr(days), function(d){ return d.key; });
        update.enter()
            .append("rect")
            .attr("height", 1).attr("y", chart.height)
            .attr("width", timescale(daysecs) - timescale(0) + 1)
            .attr("x", function(d){ return Math.floor(timescale(ts(d.key))) - 1; })
            .classed(chart.classname, true).classed("bar", true);
        update.transition()
            .attr("height", function(d){ return yscale(d.value); })
            .attr("y", function(d){ return chart.height - yscale(d.value); })
            .attr("width", timescale(daysecs) - timescale(0) + 1)
            .attr("x", function(d){ return Math.floor(timescale(ts(d.key))) - 1; })
        update.exit().transition()
            .attr("height", 0)
            .attr("y", chart.height)
            .attr("width", timescale(daysecs) - timescale(0) + 1)
            .attr("x", function(d){ return Math.floor(timescale(ts(d.key))) - 1; })
        update.on("mouseover", barover);

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
    var update = d3.select(".referrers").selectAll("tr").data(referring_domains, function(d){ return d.domain; });
    var tr = update.enter().append("tr");
    var td = tr.append("td").each(function(d){
        if(d.domain==""){
            this.appendChild(document.createTextNode("Direct link (IM, Email, Bookmarks...)"));
        } else {
            var a = document.createElement("A");
            a['href'] = "http://" + d.domain;
            a.appendChild(document.createTextNode(d.domain))
            this.appendChild(a);
            var ul = document.createElement("UL");
            d3.select(ul).selectAll("li").data(d['urls'], function(d){return d.url}).enter()
                .append("li").append("a")
                .attr("href", function(d){return d.url})
                .each(function(d){this.appendChild(document.createTextNode(d.url))});
            this.appendChild(ul);
            var thistd = this;
            a.onclick = function(e){
                e.preventDefault();
                if(thistd.className != "open"){
                    d3.select(thistd).classed("open", true);
                } else {
                    d3.select(thistd).classed("open", false);
                }
            }
        }
    })
    tr.append("td").classed("value", true).html(function(d){ return d.hits; });
    update.exit().remove();

}

function noop(){}

if(window.location.hash == "#charts"){
    var oldonload = window.onload || noop;
    window.onload = function(){ this(); initstats(); }.bind(oldonload);
}
