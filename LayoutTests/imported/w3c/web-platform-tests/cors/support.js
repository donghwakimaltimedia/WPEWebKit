// For ignoring exception names (just for testing)
/*
_real_assert_throws = assert_throws;
function assert_throws(d, func, desc) {
    try {
        func();
    } catch(e) {
        return true;
    }
    assert_unreached("Didn't throw!");
}
*/

function dirname(path) {
    return path.replace(/\/[^\/]*$/, '/')
}

/* This subdomain should point to this same location */
var SUBDOMAIN = "{{hosts[alt][]}}"
var SUBDOMAIN2 = 'www2'
var PORT = {{ports[http][1]}}
//XXX HTTPS
var PORTS = {{ports[https][0]}}

/* Changes http://example.com/abc/def/cool.htm to http://www1.example.com/abc/def/ */
var CROSSDOMAIN     = location.protocol + '//' + SUBDOMAIN + ':' + location.port + dirname(location.pathname)
var REMOTE_HOST     = SUBDOMAIN
var REMOTE_PROTOCOL = location.protocol
var REMOTE_ORIGIN   = REMOTE_PROTOCOL + '//' + REMOTE_HOST
