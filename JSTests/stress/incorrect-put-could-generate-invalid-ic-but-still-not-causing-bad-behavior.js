//@ skip if $buildType == "debug"

function shouldBe(actual, expected) {
    if (actual !== expected)
        throw new Error('bad value: ' + actual);
}

var putter = function(o) {
    o._unsupported = not_string;
}

var object;
var counter = 0;
var not_string = {
    toString() {
        counter++;
        object.ok = 42;
        return "Hey";
    }
};

for (var i = 0; i < 1000; ++i) {
    object = $vm.createObjectDoingSideEffectPutWithoutCorrectSlotStatus();
    putter(object);
}
shouldBe(counter, 1000);
