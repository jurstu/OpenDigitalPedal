# OpenDigitalPedal

## target ?

The target is to create a hardware/software combo that would allow anyone to have fun with digital processing while making it possible to use as an "effect" pedal for guitarists and other musicians. 

How do we achieve that ? 
We take rpi (3/4/5) and we add one of the [Hifi berry sound card](https://www.hifiberry.com/shop/boards/hifiberry-dac-adc/) or an [audio interface ](https://focusrite.com/products/scarlett-2i2-3rd-gen) with [low-enough latency](https://symonics.com/tested-soundcards/) and we use the [low-latency kernel raspbian build](https://github.com/guysoft/RealtimePi) to achieve <4ms input to output latency. 

# TODO
- [ ] define project structure
- [ ] add single effect structure
- [ ] define input/output dbuv(?) levels
- [ ] add startup guide
- [ ] write tests
- [ ] benchmark actual passthrough latency
- [ ] add overdrive
- [ ] add web interface
- [ ] add distortion
- [ ] add chorus
- [ ] add delay
- [ ] add vst support maybe
- [ ] design 3d case maybe
