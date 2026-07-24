#!/usr/bin/env python3
"""Generate a standalone HTML profiler visualization from profiler.json.

Usage
-----
    python3 plot_html.py -i profiler.json                # -> profiler.html
    python3 plot_html.py -i profiler.json -o result.html  # custom output path
"""

import argparse
import importlib
import json
import math
import os
import sys

# Ensure parse module is importable (same directory)
_TOOL_DIR = os.path.dirname(os.path.abspath(__file__))
if _TOOL_DIR not in sys.path:
    sys.path.insert(0, _TOOL_DIR)


def ensure_dependencies():
    """Check required third-party modules and print install guidance."""
    required = [("pandas", "pandas"), ("plotly", "plotly")]
    missing = []
    for mod, pkg in required:
        try:
            importlib.import_module(mod)
        except ImportError:
            missing.append(pkg)
    if missing:
        py = sys.executable or "python3"
        pkgs = " ".join(missing)
        print("[Dependency check] Missing required module(s):")
        for p in missing:
            print(f"  - {p}")
        print(f"\nInstall command:\n  {py} -m pip install {pkgs}")
        return False
    return True


class _NumpyEncoder(json.JSONEncoder):
    """JSON encoder that handles numpy and pandas types."""

    def default(self, obj):
        import numpy as np

        if isinstance(obj, (np.integer,)):
            return int(obj)
        if isinstance(obj, (np.floating,)):
            return float(obj)
        if isinstance(obj, np.ndarray):
            return obj.tolist()
        return super().default(obj)


# ---------------------------------------------------------------------------
# CSS
# ---------------------------------------------------------------------------

_CSS = r"""
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,sans-serif;
     background:#fff;color:#222}
#header{padding:12px 20px;border-bottom:2px solid #e0e0e0}
#header h2{margin:0;font-size:22px}
#file-info{color:#666;margin-top:4px;font-size:14px}
#controls{padding:10px 20px;background:#f8f9fa;border-bottom:1px solid #e0e0e0;
          display:flex;align-items:center;flex-wrap:wrap;gap:20px}
.ctrl-group{display:inline-flex;align-items:center;gap:8px}
.ctrl-group label{font-weight:bold;font-size:14px}
#device-checks{display:inline-flex;gap:12px}
#device-checks label{font-size:13px;cursor:pointer;display:inline-flex;
                      align-items:center;gap:3px}
#job-range{width:220px;padding:6px 8px;font-size:13px;border:1px solid #ccc;
           border-radius:4px}
#job-select{width:320px;padding:6px 8px;font-size:13px;border:1px solid #ccc;
            border-radius:4px}
#job-info{font-size:13px;color:#333}
#timeline{width:100%;min-height:400px}
.section{padding:0 20px 20px}
.section h4{margin:10px 0 4px;font-size:16px}
.section p.desc{color:#666;font-size:13px;margin:0 0 8px}
.section h5{margin:8px 0 4px;font-size:14px;color:#444}
.section h5.cpu-shared{color:#0066aa}
.tbl-wrap{display:flex;gap:30px;flex-wrap:wrap}
.tbl-card{flex:1;min-width:350px;max-width:900px}
table.stats{border-collapse:collapse;width:100%;max-width:900px;font-size:13px}
table.stats th{text-align:right;padding:6px 12px;background:#f0f0f0;
               font-family:sans-serif;font-weight:bold;border-bottom:1px solid #ddd}
table.stats th:first-child{text-align:left}
table.stats td{text-align:right;padding:6px 12px;font-family:monospace;
               border-bottom:1px solid #eee}
table.stats td:first-child{text-align:left;font-family:sans-serif}
table.stats td.sat-high{background:#ffe0e0;font-weight:bold;color:#c00}
table.stats td.sat-mid{background:#fff3e0;color:#b86e00}
"""

# ---------------------------------------------------------------------------
# JavaScript
# ---------------------------------------------------------------------------

_JS_CODE = r"""
/* ===== State ===== */
var selectedDevices = new Set(DATA.deviceIds);
var selectedJobs = new Set();
var lastClickJob = null;
var lastClickTs = 0;

/* ===== Utilities ===== */
function arrMean(a){if(!a.length)return 0;var s=0;for(var i=0;i<a.length;i++)s+=a[i];return s/a.length}
function arrStd(a){if(a.length<2)return 0;var m=arrMean(a),ss=0;for(var i=0;i<a.length;i++)ss+=(a[i]-m)*(a[i]-m);return Math.sqrt(ss/(a.length-1))}
function arrMedian(a){if(!a.length)return 0;var s=a.slice().sort(function(x,y){return x-y});var m=Math.floor(s.length/2);return s.length%2?s[m]:(s[m-1]+s[m])/2}
function arrPct(a,p){if(!a.length)return 0;var s=a.slice().sort(function(x,y){return x-y});var i=p/100*(s.length-1);var lo=Math.floor(i),hi=Math.ceil(i);return lo===hi?s[lo]:s[lo]+(s[hi]-s[lo])*(i-lo)}
function fmtNum(v,d){return(typeof v==='number')?v.toFixed(d===undefined?1:d):'—'}

function mergedBusyNs(events){
    var intervals=events.map(function(e){return[e.start_ns,e.end_ns]}).sort(function(a,b){return a[0]-b[0]});
    if(!intervals.length)return 0;
    var total=0,cs=intervals[0][0],ce=intervals[0][1];
    for(var i=1;i<intervals.length;i++){
        if(intervals[i][0]<=ce){ce=Math.max(ce,intervals[i][1])}
        else{total+=ce-cs;cs=intervals[i][0];ce=intervals[i][1]}
    }
    return total+(ce-cs);
}

function extractEventType(trackName){
    var part=trackName;
    if(trackName.indexOf(' / ')!==-1) part=trackName.split(' / ')[1];
    var idx=part.indexOf(' (');
    if(idx!==-1) part=part.substring(0,idx);
    part=part.trim();
    if(part.indexOf('Inference Core')===0) return 'Inference';
    return part;
}

/* ===== Timeline ===== */
function buildTimeline(){
    var devSet=selectedDevices;
    var jobSet=selectedJobs;
    var filtered=DATA.events.filter(function(e){return devSet.has(e.device_id)});
    if(jobSet.size>0) filtered=filtered.filter(function(e){return jobSet.has(e.job_id)});

    // User events: always included regardless of device/job filters
    var userEvts=DATA.events.filter(function(e){return e.is_user_event});
    var allFiltered=filtered.concat(userEvts);

    var subgraphs=DATA.subgraphOrder.filter(function(sg){
        return DATA.trackOrder[sg] && allFiltered.some(function(e){return e.subgraph===sg})
    });
    if(!subgraphs.length){Plotly.react('timeline',[],{height:200});return}

    var traces=[];
    var allY=[];
    for(var si=0;si<subgraphs.length;si++){
        var sg=subgraphs[si];
        var sep='\u2500\u2500 '+sg+' \u2500\u2500';
        allY.push(sep);
        var allTracks=DATA.trackOrder[sg]||[];
        var sgEvts=allFiltered.filter(function(e){return e.subgraph===sg});
        var present=new Set(sgEvts.map(function(e){return e.track}));
        var tracks=allTracks.filter(function(t){return present.has(t)});
        for(var ti=0;ti<tracks.length;ti++){
            var tn=tracks[ti];
            allY.push(tn);
            var te=sgEvts.filter(function(e){return e.track===tn});
            if(!te.length)continue;
            traces.push({
                y:te.map(function(){return tn}),
                x:te.map(function(e){return e.duration_us}),
                base:te.map(function(e){return e.start_us}),
                orientation:'h',type:'bar',
                marker:{color:te.map(function(e){return e.color}),opacity:0.85,line:{width:0}},
                customdata:te.map(function(e){return[e.job_id]}),
                hovertext:te.map(function(e){return e.hover}),
                hoverinfo:'text',showlegend:false,name:tn
            });
        }
    }
    for(var yi=0;yi<allY.length;yi++){
        if(allY[yi].indexOf('\u2500\u2500')===0){
            traces.push({y:[allY[yi]],x:[0],base:[0],orientation:'h',type:'bar',
                         marker:{color:'rgba(0,0,0,0)',line:{width:0}},
                         hoverinfo:'skip',showlegend:false});
        }
    }
    var h=Math.max(450,80+allY.length*24);
    var layout={
        barmode:'overlay',height:h,
        margin:{l:300,r:30,t:40,b:50},
        hovermode:'closest',dragmode:'pan',
        plot_bgcolor:'white',paper_bgcolor:'white',
        xaxis:{title:'Time (\u00b5s)',showgrid:true,gridcolor:'rgba(0,0,0,0.08)'},
        yaxis:{
            categoryorder:'array',
            categoryarray:allY.slice().reverse(),
            tickfont:{size:10},gridcolor:'rgba(0,0,0,0.05)',
            tickmode:'array',tickvals:allY,
            ticktext:allY.map(function(c){return c.indexOf('\u2500\u2500')===0?'<b>'+c+'</b>':c})
        }
    };
    Plotly.react('timeline',traces,layout,{scrollZoom:true,displayModeBar:true,
                  modeBarButtonsToRemove:['lasso2d','select2d']});
}

/* ===== Stats ===== */
function computeStatsFor(events){
    var groups={},order={};
    events.forEach(function(e){
        if(!groups[e.event_type]){groups[e.event_type]=[];order[e.event_type]=e.sort_index}
        groups[e.event_type].push(e.duration_us);
    });
    var rows=[];
    Object.keys(groups).forEach(function(et){
        var d=groups[et],m=arrMean(d),s=arrStd(d);
        rows.push({Stage:et,Count:d.length,
            'Avg':m,'Min':Math.min.apply(null,d),'Max':Math.max.apply(null,d),
            'p50':arrMedian(d),'p99':arrPct(d,99),
            'CV':m>0?(s/m*100):NaN,_order:order[et]});
    });
    rows.sort(function(a,b){return a._order-b._order});
    return rows;
}

function renderStatsHTML(rows,title){
    var h='<div class="tbl-card"><h5>'+title+'</h5><table class="stats"><thead><tr>';
    ['Stage','Count','Avg (\u00b5s)','Min (\u00b5s)','Max (\u00b5s)','p50 (\u00b5s)','p99 (\u00b5s)','CV (%)'].forEach(function(c){h+='<th>'+c+'</th>'});
    h+='</tr></thead><tbody>';
    rows.forEach(function(r){
        h+='<tr><td>'+r.Stage+'</td><td>'+r.Count+'</td>';
        h+='<td>'+fmtNum(r.Avg)+'</td><td>'+fmtNum(r.Min)+'</td>';
        h+='<td>'+fmtNum(r.Max)+'</td><td>'+fmtNum(r.p50)+'</td>';
        h+='<td>'+fmtNum(r.p99)+'</td><td>'+fmtNum(r.CV)+'</td></tr>';
    });
    h+='</tbody></table></div>';
    return h;
}

function updateStats(){
    var filtered=DATA.events.filter(function(e){return selectedDevices.has(e.device_id)});
    var el=document.getElementById('stats-container');
    if(selectedJobs.size>0){
        var jobEvts=filtered.filter(function(e){return selectedJobs.has(e.job_id)});
        var n=selectedJobs.size;
        el.innerHTML=renderStatsHTML(computeStatsFor(jobEvts),
            'Selected '+n+' Job'+(n>1?'s':''));
    }else{
        var devs=[...selectedDevices].sort(function(a,b){return a-b});
        var html='<div class="tbl-wrap">';
        devs.forEach(function(d){
            var de=filtered.filter(function(e){return e.device_id===d});
            if(!de.length)return;
            html+=renderStatsHTML(computeStatsFor(de),d<0?'CPU':'Device '+d);
        });
        html+='</div>';
        el.innerHTML=html;
    }
    // User Events statistics (always shown)
    var userEvts=DATA.events.filter(function(e){return e.is_user_event});
    if(userEvts.length>0){
        var userRows=computeStatsFor(userEvts);
        if(userRows.length>0){
            el.innerHTML+=renderStatsHTML(userRows,'User Events');
        }
    }
}

/* ===== Utilization ===== */
function isCpuStage(et,sg){
    return(sg&&sg.indexOf('cpu_')===0)||et.indexOf('cpu_')===0;
}

function computeUtilForDevice(devId){
    var excluded=new Set(['NPU Task','CPU Dispatch Wait']);
    var devEvts=DATA.events.filter(function(e){return e.device_id===devId});
    if(!devEvts.length)return null;
    var wallStart=Infinity,wallEnd=-Infinity;
    devEvts.forEach(function(e){if(e.start_ns<wallStart)wallStart=e.start_ns;if(e.end_ns>wallEnd)wallEnd=e.end_ns});
    var wallNs=wallEnd-wallStart;
    if(wallNs<=0)return null;

    var stageOrder={};
    devEvts.forEach(function(e){if(!(e.event_type in stageOrder)||e.sort_index<stageOrder[e.event_type])stageOrder[e.event_type]=e.sort_index});

    var rows=[];
    var etypes=Object.keys(stageOrder).sort(function(a,b){return stageOrder[a]-stageOrder[b]});
    etypes.forEach(function(et){
        if(excluded.has(et))return;
        var etEvts=devEvts.filter(function(e){return e.event_type===et});
        if(!etEvts.length)return;
        if(isCpuStage(et,etEvts[0].subgraph))return;
        var channels={};
        etEvts.forEach(function(e){var ch=e.sub_id||'\u2014';if(!channels[ch])channels[ch]=[];channels[ch].push(e)});
        var chUtils=[];
        Object.keys(channels).forEach(function(ch){
            var busy=mergedBusyNs(channels[ch]);
            chUtils.push(Math.round(busy/wallNs*1000)/10);
        });
        if(!chUtils.length)return;
        rows.push({Stage:et,Sat:Math.max.apply(null,chUtils),Avg:Math.round(chUtils.reduce(function(s,v){return s+v},0)/chUtils.length*10)/10});
    });
    return{rows:rows,wallMs:Math.round(wallNs/1e6*10)/10};
}

function computeSharedCpuUtil(){
    var excluded=new Set(['NPU Task']);
    var cpuEvts=DATA.events.filter(function(e){
        return(e.subgraph&&e.subgraph.indexOf('cpu_')===0)||e.event_type.indexOf('cpu_')===0;
    });
    if(!cpuEvts.length)return null;
    var wallStart=Infinity,wallEnd=-Infinity;
    DATA.events.forEach(function(e){if(e.start_ns<wallStart)wallStart=e.start_ns;if(e.end_ns>wallEnd)wallEnd=e.end_ns});
    var wallNs=wallEnd-wallStart;
    if(wallNs<=0)return null;

    var stageOrder={};
    cpuEvts.forEach(function(e){if(!(e.event_type in stageOrder)||e.sort_index<stageOrder[e.event_type])stageOrder[e.event_type]=e.sort_index});

    var rows=[];
    Object.keys(stageOrder).sort(function(a,b){return stageOrder[a]-stageOrder[b]}).forEach(function(et){
        if(excluded.has(et))return;
        var etEvts=cpuEvts.filter(function(e){return e.event_type===et});
        var channels={};
        etEvts.forEach(function(e){var ch=e.sub_id||'\u2014';if(!channels[ch])channels[ch]=[];channels[ch].push(e)});
        var chUtils=[];
        Object.keys(channels).forEach(function(ch){
            var busy=mergedBusyNs(channels[ch]);
            chUtils.push(Math.round(busy/wallNs*1000)/10);
        });
        if(!chUtils.length)return;
        rows.push({Stage:et,Sat:Math.max.apply(null,chUtils),Avg:Math.round(chUtils.reduce(function(s,v){return s+v},0)/chUtils.length*10)/10});
    });
    if(!rows.length)return null;
    return{rows:rows,wallMs:Math.round(wallNs/1e6*10)/10};
}

function renderUtilHTML(rows,title){
    var h='<div class="tbl-card"><h5>'+title+'</h5><table class="stats"><thead><tr>';
    ['Stage','Saturation (%)','Avg Util (%)'].forEach(function(c){h+='<th>'+c+'</th>'});
    h+='</tr></thead><tbody>';
    rows.forEach(function(r){
        var satCls='';
        if(r.Sat>=90)satCls=' class="sat-high"';
        else if(r.Sat>=70)satCls=' class="sat-mid"';
        h+='<tr><td>'+r.Stage+'</td><td'+satCls+'>'+r.Sat+'</td><td>'+r.Avg+'</td></tr>';
    });
    h+='</tbody></table></div>';
    return h;
}

function updateUtil(){
    var el=document.getElementById('util-container');
    var devs=[...selectedDevices].sort(function(a,b){return a-b});
    var html='<div class="tbl-wrap">';
    devs.forEach(function(d){
        if(d<0)return;
        var res=computeUtilForDevice(d);
        if(!res||!res.rows.length)return;
        html+=renderUtilHTML(res.rows,(d<0?'CPU':'Device '+d)+'  (wall time: '+res.wallMs+' ms)');
    });
    var shared=computeSharedCpuUtil();
    if(shared&&shared.rows.length){
        html+='<div class="tbl-card"><h5 class="cpu-shared">Shared CPU Resources \u2014 All Devices  (wall time: '+shared.wallMs+' ms)</h5>';
        html+='<table class="stats"><thead><tr><th>Stage</th><th>Saturation (%)</th><th>Avg Util (%)</th></tr></thead><tbody>';
        shared.rows.forEach(function(r){
            var satCls='';
            if(r.Sat>=90)satCls=' class="sat-high"';
            else if(r.Sat>=70)satCls=' class="sat-mid"';
            html+='<tr><td>'+r.Stage+'</td><td'+satCls+'>'+r.Sat+'</td><td>'+r.Avg+'</td></tr>';
        });
        html+='</tbody></table></div>';
    }
    html+='</div>';
    el.innerHTML=html;
}

/* ===== Transition Latency ===== */
function computeTransitions(jobFilter,deviceFilter){
    var gaps={},order={},idx=0;
    var targetJobs=jobFilter?[...jobFilter]:Object.keys(DATA.flowData).map(Number);
    targetJobs.forEach(function(jid){
        if(deviceFilter&&!deviceFilter.has(DATA.jobToDevice[jid]))return;
        var flows=DATA.flowData[jid]||[];
        flows.forEach(function(f){
            var from=extractEventType(f.track_from);
            var to=extractEventType(f.track_to);
            var label=from+' \u2192 '+to;
            var gap=f.start_us-f.end_us;
            if(!gaps[label]){gaps[label]=[];order[label]=idx++}
            gaps[label].push(gap);
        });
    });
    if(!Object.keys(gaps).length)return[];
    var rows=[];
    Object.keys(order).sort(function(a,b){return order[a]-order[b]}).forEach(function(label){
        var v=gaps[label];
        rows.push({Transition:label,Count:v.length,Avg:arrMean(v),Min:Math.min.apply(null,v),Max:Math.max.apply(null,v),Median:arrMedian(v)});
    });
    return rows;
}

function renderTransHTML(rows,title){
    var h='<div class="tbl-card"><h5>'+title+'</h5><table class="stats"><thead><tr>';
    ['Transition','Count','Avg (\u00b5s)','Min (\u00b5s)','Max (\u00b5s)','Median (\u00b5s)'].forEach(function(c){h+='<th>'+c+'</th>'});
    h+='</tr></thead><tbody>';
    rows.forEach(function(r){
        h+='<tr><td>'+r.Transition+'</td><td>'+r.Count+'</td>';
        h+='<td>'+fmtNum(r.Avg)+'</td><td>'+fmtNum(r.Min)+'</td>';
        h+='<td>'+fmtNum(r.Max)+'</td><td>'+fmtNum(r.Median)+'</td></tr>';
    });
    h+='</tbody></table></div>';
    return h;
}

function updateTransitions(){
    var el=document.getElementById('transition-container');
    if(selectedJobs.size>0){
        var rows=computeTransitions(selectedJobs,null);
        if(rows.length){
            var n=selectedJobs.size;
            el.innerHTML=renderTransHTML(rows,'Selected '+n+' Job'+(n>1?'s':''));
        }else{el.innerHTML=''}
    }else{
        var devs=[...selectedDevices].sort(function(a,b){return a-b});
        var html='<div class="tbl-wrap">';
        devs.forEach(function(d){
            var rows=computeTransitions(null,new Set([d]));
            if(!rows.length)return;
            html+=renderTransHTML(rows,d<0?'CPU':'Device '+d);
        });
        html+='</div>';
        el.innerHTML=html;
    }
}

/* ===== Controls ===== */
function setupDeviceFilter(){
    var wrap=document.getElementById('device-checks');
    DATA.deviceIds.forEach(function(d){
        var lbl=document.createElement('label');
        var cb=document.createElement('input');
        cb.type='checkbox';cb.checked=true;cb.value=d;
        cb.addEventListener('change',function(){
            if(this.checked)selectedDevices.add(d);else selectedDevices.delete(d);
            updateView();
        });
        lbl.appendChild(cb);
        lbl.appendChild(document.createTextNode(d<0?' CPU':' Device '+d));
        wrap.appendChild(lbl);
    });
}

function parseJobRange(text){
    var valid=new Set(DATA.jobIds);
    var result=new Set();
    if(!text||!text.trim())return result;
    text.split(',').forEach(function(part){
        part=part.trim();
        if(!part)return;
        if(part.indexOf('-')!==-1){
            var bounds=part.split('-');
            var lo=parseInt(bounds[0]),hi=parseInt(bounds[1]);
            if(!isNaN(lo)&&!isNaN(hi)){for(var j=lo;j<=hi;j++){if(valid.has(j))result.add(j)}}
        }else{
            var j=parseInt(part);
            if(!isNaN(j)&&valid.has(j))result.add(j);
        }
    });
    return result;
}

function compressJobRange(jobs){
    if(!jobs.length)return'';
    var sorted=jobs.slice().sort(function(a,b){return a-b});
    var ranges=[],start=sorted[0],end=sorted[0];
    for(var i=1;i<sorted.length;i++){
        if(sorted[i]<=end+2){end=sorted[i]}
        else{ranges.push(end>start?start+'-'+end:''+start);start=end=sorted[i]}
    }
    ranges.push(end>start?start+'-'+end:''+start);
    return ranges.join(', ');
}

function setupJobFilter(){
    var rangeInput=document.getElementById('job-range');
    var selectEl=document.getElementById('job-select');
    // Populate select options
    DATA.jobIds.forEach(function(j){
        var opt=document.createElement('option');
        opt.value=j;opt.textContent='Job '+j;
        selectEl.appendChild(opt);
    });
    // Range input -> update selection
    rangeInput.addEventListener('change',function(){
        var jobs=parseJobRange(this.value);
        selectedJobs=jobs;
        // Sync multi-select
        for(var i=0;i<selectEl.options.length;i++){
            selectEl.options[i].selected=jobs.has(parseInt(selectEl.options[i].value));
        }
        updateView();
    });
    // Multi-select -> update range input
    selectEl.addEventListener('change',function(){
        var jobs=new Set();
        for(var i=0;i<this.selectedOptions.length;i++){
            jobs.add(parseInt(this.selectedOptions[i].value));
        }
        selectedJobs=jobs;
        rangeInput.value=compressJobRange([...jobs]);
        updateView();
    });
}

function syncJobControls(){
    var rangeInput=document.getElementById('job-range');
    var selectEl=document.getElementById('job-select');
    rangeInput.value=compressJobRange([...selectedJobs]);
    for(var i=0;i<selectEl.options.length;i++){
        selectEl.options[i].selected=selectedJobs.has(parseInt(selectEl.options[i].value));
    }
}

function setupClickHandlers(){
    var el=document.getElementById('timeline');
    el.on('plotly_click',function(data){
        if(!data||!data.points||!data.points.length)return;
        var custom=data.points[0].customdata;
        if(!custom||!custom.length)return;
        var jobId=custom[0];
        var now=Date.now();
        var isDbl=(lastClickJob===jobId&&(now-lastClickTs)<400);
        lastClickJob=jobId;lastClickTs=now;
        if(isDbl){
            if(selectedJobs.size===1&&selectedJobs.has(jobId)){selectedJobs.clear()}
            else{selectedJobs.clear();selectedJobs.add(jobId)}
        }else{
            if(selectedJobs.has(jobId)){selectedJobs.delete(jobId)}
            else{selectedJobs.add(jobId)}
        }
        syncJobControls();
        updateView();
    });
}

/* ===== Main Update ===== */
function updateView(){
    buildTimeline();
    updateStats();
    updateUtil();
    updateTransitions();
    // Job info
    var infoEl=document.getElementById('job-info');
    if(selectedJobs.size>0){
        var parts=[];
        selectedJobs.forEach(function(j){
            var d=DATA.jobToDevice[j];
            parts.push('Job '+j+' (Dev '+(d!==undefined?d:'?')+')');
        });
        infoEl.textContent='Selected: '+parts.join(', ');
    }else{infoEl.textContent=''}
}

/* ===== Init ===== */
document.addEventListener('DOMContentLoaded',function(){
    setupDeviceFilter();
    setupJobFilter();
    updateView();
    setupClickHandlers();
});
"""

# ---------------------------------------------------------------------------
# HTML body template
# ---------------------------------------------------------------------------

_HTML_BODY = """
<div id="header">
  <h2>DX-RT Profiler Viewer</h2>
  <div id="file-info">{file_info}</div>
</div>
<div id="controls">
  <div class="ctrl-group">
    <label>Device:</label>
    <div id="device-checks"></div>
  </div>
  <div class="ctrl-group">
    <label>Highlight Jobs:</label>
    <input id="job-range" type="text" placeholder="e.g. 200-219, 500, 510-515">
    <select id="job-select" multiple></select>
  </div>
  <span id="job-info"></span>
</div>
<div id="timeline"></div>
<div class="section">
  <h4>Pipeline Statistics</h4>
  <div id="stats-container"></div>
</div>
<div class="section">
  <h4>Pipeline Bottleneck Analysis</h4>
  <p class="desc">Saturation = max utilization among all channels (or threads) of a stage.
  Avg Util = mean utilization across channels.
  Stages with Saturation near 100% are pipeline bottlenecks.</p>
  <div id="util-container"></div>
</div>
<div class="section">
  <h4>Stage Transition Latency</h4>
  <p class="desc">Time gap between the end of one stage and the start of the next
  stage within the same job's pipeline flow.</p>
  <div id="transition-container"></div>
</div>
"""


# ---------------------------------------------------------------------------
# HTML generation
# ---------------------------------------------------------------------------

def generate_html(input_path, output_path):
    """Generate a standalone HTML profiler visualization."""
    from parse import load_profiler
    import plotly
    import pandas as pd

    print(f"Loading {input_path} ...")
    data = load_profiler(input_path)
    df = data["df"]

    n_events = len(df)
    n_jobs = len(data["job_ids"])
    n_devices = len(data["device_ids"])
    sg_str = " \u2192 ".join(data["subgraph_order"])
    print(f"  {n_events:,} events | {n_jobs} jobs | "
          f"{n_devices} devices | Pipeline: {sg_str}")

    # Convert DataFrame to JSON-serializable list
    keep_cols = [
        "event_type", "device_id", "job_id", "subgraph", "sub_id",
        "start_ns", "end_ns", "duration_ns",
        "start_us", "end_us", "duration_us",
        "track", "sort_index", "color", "hover",
        "is_user_event",
    ]
    events_df = df[keep_cols].copy()
    events_df = events_df.where(pd.notna(events_df), None)
    events_list = events_df.to_dict("records")

    # Fix types for JSON (numpy int/float -> Python int/float)
    for evt in events_list:
        for k, v in evt.items():
            if hasattr(v, "item"):
                evt[k] = v.item()
            elif v is None:
                pass
            elif k in ("device_id", "job_id", "sort_index") and v is not None:
                if isinstance(v, float) and math.isnan(v):
                    evt[k] = None
                else:
                    evt[k] = int(v)

    # Prepare metadata dict
    # Convert flow_data keys from int to string for JSON
    flow_data_str = {}
    for jid, flows in data["flow_data"].items():
        flow_data_str[int(jid)] = flows

    embedded_data = {
        "events": events_list,
        "trackOrder": data["track_order"],
        "deviceIds": [int(d) for d in data["device_ids"]],
        "jobIds": [int(j) for j in data["job_ids"]],
        "jobColors": {int(k): v for k, v in data["job_colors"].items()},
        "jobToDevice": {int(k): int(v)
                        for k, v in data["job_to_device"].items()},
        "flowData": flow_data_str,
        "subgraphOrder": data["subgraph_order"],
    }

    data_json = json.dumps(embedded_data, cls=_NumpyEncoder,
                           ensure_ascii=False)

    # Get plotly.js source
    plotly_js = plotly.offline.get_plotlyjs()

    # File info text
    file_info = (
        f"{os.path.basename(input_path)}  \u2022  "
        f"{n_events:,} events  \u2022  {n_jobs} jobs  \u2022  "
        f"{n_devices} device(s)  \u2022  Pipeline: {sg_str}"
    )

    # Assemble HTML
    body = _HTML_BODY.format(file_info=file_info)
    html = f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>DX-RT Profiler Viewer</title>
<script>{plotly_js}</script>
<style>{_CSS}</style>
</head>
<body>
{body}
<script>
var DATA = {data_json};
{_JS_CODE}
</script>
</body>
</html>"""

    with open(output_path, "w", encoding="utf-8") as f:
        f.write(html)

    size_mb = os.path.getsize(output_path) / (1024 * 1024)
    print(f"  Generated: {output_path} ({size_mb:.1f} MB)")


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    if not ensure_dependencies():
        sys.exit(1)

    parser = argparse.ArgumentParser(
        description="Generate standalone HTML profiler visualization.",
    )
    parser.add_argument(
        "-i", "--input", required=True,
        help="Input profiler.json file path",
    )
    parser.add_argument(
        "-o", "--output", default=None,
        help="Output HTML file path (default: same name with .html extension)",
    )
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(0)

    args = parser.parse_args()
    out = args.output
    if out is None:
        base = os.path.splitext(args.input)[0]
        out = base + ".html"

    generate_html(args.input, out)
