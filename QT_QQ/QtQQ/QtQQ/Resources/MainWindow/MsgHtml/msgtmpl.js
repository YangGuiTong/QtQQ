var external0 = null;
var external_10001 = null;
var external_10002 = null;
var external_10003 = null;
var external_10004 = null;
var external_10005 = null;
var external_10006 = null;
var external_10007 = null;
var external_10008 = null;
var external_10009 = null;



String.prototype.format = function() {  
    if(arguments.length == 0) return this;  
    var obj = arguments[0];  
    var s = this;  
    for(var key in obj) {  
        s = s.replace(new RegExp("\\{\\{" + key + "\\}\\}", "g"), obj[key]);  
    }  
    return s;  
}  

new QWebChannel(qt.webChannelTransport,
	function(channel) {
		external0 = channel.objects.external0;
		external_10001 = channel.objects.external_10001;
new QWebChannel(qt.webChannelTransport,			function(channel) {			external_10001 = channel.objects.external_10001;		}		); 		
function recvHtml_10001(msg){
	$("#placeholder").append(external_10001.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
external_10002 = channel.objects.external_10002;
new QWebChannel(qt.webChannelTransport,			function(channel) {			external_10002 = channel.objects.external_10002;		}		); 		
function recvHtml_10002(msg){
	$("#placeholder").append(external_10002.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
external_10003 = channel.objects.external_10003;
new QWebChannel(qt.webChannelTransport,			function(channel) {			external_10003 = channel.objects.external_10003;		}		); 		
function recvHtml_10003(msg){
	$("#placeholder").append(external_10003.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
external_10004 = channel.objects.external_10004;
new QWebChannel(qt.webChannelTransport,			function(channel) {			external_10004 = channel.objects.external_10004;		}		); 		
function recvHtml_10004(msg){
	$("#placeholder").append(external_10004.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
external_10005 = channel.objects.external_10005;
new QWebChannel(qt.webChannelTransport,			function(channel) {			external_10005 = channel.objects.external_10005;		}		); 		
function recvHtml_10005(msg){
	$("#placeholder").append(external_10005.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
external_10006 = channel.objects.external_10006;
new QWebChannel(qt.webChannelTransport,			function(channel) {			external_10006 = channel.objects.external_10006;		}		); 		
function recvHtml_10006(msg){
	$("#placeholder").append(external_10006.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
external_10007 = channel.objects.external_10007;
new QWebChannel(qt.webChannelTransport,			function(channel) {			external_10007 = channel.objects.external_10007;		}		); 		
function recvHtml_10007(msg){
	$("#placeholder").append(external_10007.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
external_10008 = channel.objects.external_10008;
new QWebChannel(qt.webChannelTransport,			function(channel) {			external_10008 = channel.objects.external_10008;		}		); 		
function recvHtml_10008(msg){
	$("#placeholder").append(external_10008.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
external_10009 = channel.objects.external_10009;
new QWebChannel(qt.webChannelTransport,			function(channel) {			external_10009 = channel.objects.external_10009;		}		); 		
function recvHtml_10009(msg){
	$("#placeholder").append(external_10009.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};

		
	}
);

function appendHtml0(msg){
	$("#placeholder").append(external0.msgRHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};

