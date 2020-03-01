using Toybox.Activity as Act;
using Toybox.Application as App;
using Toybox.Graphics as Gfx;
using Toybox.Lang;
using Toybox.System as Sys;
using Toybox.Time;
using Toybox.WatchUi as Ui;
using Toybox.Communications as Comms;

class OrangeCedarView extends Ui.View {

    var response;
    var fonts = {};

    function initialize() {
        View.initialize();
        fonts["NORMAL"] = Ui.loadResource(Rez.Fonts.SegoeUi24Bold);
    }

    function onShow() {
        makeRequest();
    }

    function onUpdate(dc) {
        dc.setColor(Gfx.COLOR_BLACK, Gfx.COLOR_BLACK);
        dc.clear();
        
        if (response) {
	        dc.setColor(Gfx.COLOR_WHITE, Gfx.COLOR_TRANSPARENT);
            for (var i = 0; i < response.size(); ++i) {
                Sys.println(response[i]);
		        dc.drawText(
		            dc.getWidth() / 2,
		            dc.getHeight() * (i + 1) / (response.size() + 1) - 9,
		            fonts["NORMAL"],
		            response[i]["name"],
		            Gfx.TEXT_JUSTIFY_CENTER | Gfx.TEXT_JUSTIFY_VCENTER);
		        dc.drawText(
		            dc.getWidth() / 2,
		            dc.getHeight() * (i + 1) / (response.size() + 1) + 9,
		            fonts["NORMAL"],
		            response[i]["value"],
		            Gfx.TEXT_JUSTIFY_CENTER | Gfx.TEXT_JUSTIFY_VCENTER);
            }
        } else {
	        var clockTime = System.getClockTime();
	        dc.setColor(Gfx.COLOR_WHITE, Gfx.COLOR_TRANSPARENT);
	        dc.drawText(
	            dc.getWidth() / 2,
	            dc.getHeight() / 2,
	            Gfx.FONT_MEDIUM,
	            Lang.format("$1$ $2$ $3$", [clockTime.hour.format("%02d"), clockTime.min.format("%02d"), clockTime.sec.format("%02d")]),
	            Gfx.TEXT_JUSTIFY_CENTER | Gfx.TEXT_JUSTIFY_VCENTER);
        }
    }

	function makeRequest() {
	   var url = "https://api.walterheger.de/orangecedar/garmin.php"; // must be https
	   var params = { /* "param" => "1234" */ };
	   var options = {
		   :method => Comms.HTTP_REQUEST_METHOD_GET,
		   // :headers => { "Content-Type" => Communications.REQUEST_CONTENT_TYPE_JSON },
		   :responseType => Comms.HTTP_RESPONSE_CONTENT_TYPE_JSON
	   };
	   Comms.makeWebRequest(url, params, options, method(:onReceive));
	}

    function onReceive(responseCode, data) {
        if (responseCode == 200) {
            Sys.println(data);
            response = data;
        }
        else {
            Sys.println("Response: " + responseCode);
        }
        Ui.requestUpdate();
    }

    function onLayout(dc) {
        setLayout(Rez.Layouts.MainLayout(dc));
    }

    function onHide() {
    }
}
