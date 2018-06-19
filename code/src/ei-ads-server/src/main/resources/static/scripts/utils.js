String.prototype.format = function(args) {
    var result = this;
    if (arguments.length > 0) {    
        if (arguments.length == 1 && typeof (args) == "object") {
            for (var key in args) {
                if(args[key]!=undefined){
                    var reg = new RegExp("({" + key + "})", "g");
                    result = result.replace(reg, args[key]);
                }
            }
        }
        else {
            for (var i = 0; i < arguments.length; i++) {
                if (arguments[i] != undefined) {
                    var reg = new RegExp("({[" + i + "]})", "g");
                    result = result.replace(reg, arguments[i]);
                }
            }
        }
    }
    return result;
}



const BYTE_COUNT = {KB:1024, MB:1024*1024, GB: 1024*1024*1024};

function formateByte(count, fixed, unit) {
	if (fixed == undefined) {
		fixed = 1;
	}

	if (count == 0)
		return 0;
	
	//var unit;
	var value;
	var unitFormater = {
		bytes : function(count) { return count; },
	    KB : function(count) { return (count * 1.0/(BYTE_COUNT.KB)).toFixed(fixed); },
		MB : function(count) { return (count * 1.0/(BYTE_COUNT.MB)).toFixed(fixed); },
	    GB : function(count) { return (count * 1.0/(BYTE_COUNT.GB)).toFixed(fixed); },
	}
	if (unit == undefined) {
		if (count < BYTE_COUNT.KB) {
			unit = "bytes"
			//value = count;			
		} else if (count < BYTE_COUNT.MB) {
			//value = (count * 1.0/(BYTE_COUNT.KB)).toFixed(fixed);
			unit = "KB";
		} else if (count < BYTE_COUNT.GB) {
			//value = (count * 1.0/(BYTE_COUNT.MB)).toFixed(fixed);
			unit = "MB";
		} else {
			//value = (count * 1.0/(BYTE_COUNT.GB)).toFixed(fixed);
			unit = "GB";
	    }
		
		return unitFormater[unit](count)+" "+unit;
	} else {
		return unitFormater[unit](count);
	}
	
}

function str_repeat(i, m) {
    for (var o = []; m > 0; o[--m] = i);
    return o.join('');
}

function sprintf() {
    var i = 0, a, f = arguments[i++], o = [], m, p, c, x, s = '';
    while (f) {
        if (m = /^[^\x25]+/.exec(f)) {
            o.push(m[0]);
        }
        else if (m = /^\x25{2}/.exec(f)) {
            o.push('%');
        }
        else if (m = /^\x25(?:(\d+)\$)?(\+)?(0|'[^$])?(-)?(\d+)?(?:\.(\d+))?([b-fosuxX])/.exec(f)) {
            if (((a = arguments[m[1] || i++]) == null) || (a == undefined)) {
                throw('Too few arguments.');
            }
            if (/[^s]/.test(m[7]) && (typeof(a) != 'number')) {
                throw('Expecting number but found ' + typeof(a));
            }
            switch (m[7]) {
                case 'b': a = a.toString(2); break;
                case 'c': a = String.fromCharCode(a); break;
                case 'd': a = parseInt(a); break;
                case 'e': a = m[6] ? a.toExponential(m[6]) : a.toExponential(); break;
                case 'f': a = m[6] ? parseFloat(a).toFixed(m[6]) : parseFloat(a); break;
                case 'o': a = a.toString(8); break;
                case 's': a = ((a = String(a)) && m[6] ? a.substring(0, m[6]) : a); break;
                case 'u': a = Math.abs(a); break;
                case 'x': a = a.toString(16); break;
                case 'X': a = a.toString(16).toUpperCase(); break;
            }
            a = (/[def]/.test(m[7]) && m[2] && a >= 0 ? '+'+ a : a);
            c = m[3] ? m[3] == '0' ? '0' : m[3].charAt(1) : ' ';
            x = m[5] - String(a).length - s.length;
            p = m[5] ? str_repeat(c, x) : '';
            o.push(s + (m[4] ? a + p : p + a));
        }
        else {
            throw('Huh ?!');
        }
        f = f.substring(m[0].length);
    }
    return o.join('');
}


// 对Date的扩展，将 Date 转化为指定格式的String
// 月(M)、日(d)、小时(h)、分(m)、秒(s)、季度(q) 可以用 1-2 个占位符， 
// 年(y)可以用 1-4 个占位符，毫秒(S)只能用 1 个占位符(是 1-3 位的数字) 
// 例子： 
// (new Date()).Format("yyyy-MM-dd hh:mm:ss.S") ==> 2006-07-02 08:09:04.423 
// (new Date()).Format("yyyy-M-d h:m:s.S")      ==> 2006-7-2 8:9:4.18 
Date.prototype.Format = function (fmt) { //author: meizz 
    var o = {
        "M+": this.getMonth() + 1, //月份 
        "d+": this.getDate(), //日 
        "h+": this.getHours(), //小时 
        "H+": this.getHours(),
        "m+": this.getMinutes(), //分 
        "s+": this.getSeconds(), //秒 
        "q+": Math.floor((this.getMonth() + 3) / 3), //季度 
        "S+": this.getMilliseconds() //毫秒 
    };
    if (/(y+)/.test(fmt)) fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
    for (var k in o)
       if (new RegExp("(" + k + ")").test(fmt)) {
    	   var str = [];
    	   for (var i = 0; i < RegExp.$1.length; i++) {
    		   str.push('0');
    	   }
    	   
    	   fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : ((str.join('') + o[k]).substr(("" + o[k]).length)));
       }
    return fmt;
}

function ResultFunction(rt) {
	if (rt.code == 'FAIL') {
		alert(rt.detail);
		
	} else if (rt.code == 'OK') {
		
		
		return rt.value;
	} else {
		console.log(rt.detail);
		
	}
}

var AjaxFunction =  function (url, callBack, failCallBack) {
     var result;
     
	 jQuery.ajax({
	        url: this.baseUrl+url,
	        success: callBack != undefined ? callBack : function(record) { result = record; },
	        async: callBack == undefined ? false : true,
	        error: failCallBack	== undefined ? function(e){console.log(e);} : failCallBack		        
	    });
	 
	 
	 if (callBack == undefined) {
		 return result;
	 }
};	

function ModelFunction() {
	this.baseUrl = '/model/';
	this.getAll = function(callBack, failCallBack) { return this.query('all', callBack, failCallBack);};
	this.getRoots =  function(callBack, failCallBack) { return this.query('roots', callBack, failCallBack);};
}  

ModelFunction.prototype.query =  AjaxFunction;

modelApi = new ModelFunction();

function NodeFunction() {
	this.baseUrl = 'node/';
	this.getNodes = function(callBack, failCallBack) { return this.query('all', callBack, failCallBack);};
}  

NodeFunction.prototype.query = AjaxFunction;

var nodeApi = new NodeFunction();

function JobFunction() {
	this.baseUrl = 'job/';
	this.getJobs = function(callBack, failCallBack) { return this.query('all', callBack, failCallBack);};	
	this.newJobName = function(callBack, failCallBack) { return this.query('newjobname', callBack, failCallBack);};
	//this.getConfig = function(jobType, callBack, failCallBack) { return this.query("config/"+jobType, callBack, failCallBack);};
}

JobFunction.prototype.query = AjaxFunction;

var jobApi = new JobFunction();


function AdsFunctions() {
	this.baseUrl = '';
	this.getDefaultProperty = function(type) { return this.query("/property/default/"+type);};
	this.getPropertyNames = function(type) { return this.query("/property/names/"+type);};
	this.findPropertyName = function(type, name) { return this.query("/property/name/"+type+"/"+name);};
}

AdsFunctions.prototype.query = AjaxFunction;

var adsApi = new AdsFunctions();


