    freeboard.addStyle('.gm-style-cc a', "text-shadow:none;");

    var multiMapWidget = function (settings) {
        var self = this;
        var currentSettings = settings;
        var map;
        var markers;
        var currentPoints = [];

        function updatePosition() {
            if (map && markers && currentPoints) {
				markerBounds = new google.maps.LatLngBounds();
				var i;
				for (i = 0; i < currentPoints.length; ++i) {
					if (!markers[i])
						markers[i] = new google.maps.Marker({map: map});
					newLatLon = new google.maps.LatLng(currentPoints[i].lat, currentPoints[i].lon);
					markers[i].setPosition(newLatLon);
					markerBounds.extend(newLatLon);
				}
                map.fitBounds(markerBounds);
            }
        }

        this.render = function (element) {
            function initializeMap() {
                var mapOptions = {
                    zoom: 13,
                    center: new google.maps.LatLng(37.235, -115.811111),
                    disableDefaultUI: true,
                    draggable: false
                };

                map = new google.maps.Map(element, mapOptions);

                google.maps.event.addDomListener(element, 'mouseenter', function (e) {
                    e.cancelBubble = true;
                    if (!map.hover) {
                        map.hover = true;
                        map.setOptions({zoomControl: true});
                    }
                });

                google.maps.event.addDomListener(element, 'mouseleave', function (e) {
                    if (map.hover) {
                        map.setOptions({zoomControl: false});
                        map.hover = false;
                    }
                });


                updatePosition();
            }

            if (window.google && window.google.maps) {
                initializeMap();
            }
            else {
                window.gmap_initialize = initializeMap;
                head.js("https://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false&callback=gmap_initialize");
            }
        }

        this.onSettingsChanged = function (newSettings) {
            currentSettings = newSettings;
        }

        this.onCalculatedValueChanged = function (settingName, newValue) {
            if (settingName == "points") {
                currentPoints = newValue;
            }
            updatePosition();
        }

        this.onDispose = function () {
        }

        this.getHeight = function () {
            return 4;
        }

        this.onSettingsChanged(settings);
    };

    freeboard.loadWidgetPlugin({
        type_name: "multi_map",
        display_name: "Multi Google Map",
        fill_size: true,
        settings: [
            {
				name: "points",
                display_name: "Points as [{lon:xxx, lat:yyy},{....]",
                type: "calculated"
            }
        ],
        newInstance: function (settings, newInstanceCallback) {
            newInstanceCallback(new multiMapWidget(settings));
        }
    });