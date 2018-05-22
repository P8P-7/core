# Structure
## Concepts
The `core` is divided into a `controller` and many `modules`. 
### Modules
The modules represent wrappers around physical objects and/or libraries. Often they are an abstraction of an otherwise tedious or complicated task. For example, the `vision` library makes it easy to detect lines within an image without first processing canny image's or connecting parts. 
### Controller
The controller consists of three parts, `handles`, `commands` and `repositories`.

**Handles**
Handles are a way for the controller to define whether a physical or digital resource is available or not. Every module can define it's own handles. These handles can also have other data associated with them, like id's or configuration. Handles are like mutexes in that they can be locked and unlocked to represent the state of a resource. Handles are stored in a `HandleMap` with an associated ID.

**Commands**
The commands represent the actual logic of the core. They contain a list of instructions for a specific task. Commands are stored within a `CommandMap`. Before adding a command to said map, the creator must assign all handles the current command _can_ occupy. All these handles will be locked when the command is called. And can be unlocked dynamically.

**Repositories**

##Application Flow
When a command enters the appliation via the `ZMQ Messaging Module` it will be looked up in the `HandleMap` and it's passed arguments extracted. All necessary handles will be locked and the command will be started on a new thread. Within this new thread the command can unlock all handles itself. Whilst running the command can dynamically check if a other command is trying to interrupt itself. If so, the command can handle the requried steps itself. Else the command will finish executing and unlock all occupied handles.

## Layout
### Project Map

```
|-- cmake
|   `-- modules
|-- doc
|-- src
|   |-- controller
|   |   |-- commands
|   |   `-- util
|   |-- modules
|   |   |-- foundation
|   |   |   |-- include
|   |   |   |   `-- goliath
|   |   |   `-- src
|   |   |-- gpio
|   |   |   `-- include
|   |   |       `-- goliath
|   |   |-- i2c
|   |   |   |-- handles
|   |   |   `-- include
|   |   |       `-- goliath
|   |   |-- motor-controller
|   |   |   |-- include
|   |   |   |   `-- goliath
|   |   |   `-- sketches
|   |   |-- openal
|   |   |   |-- include
|   |   |   |   `-- goliath
|   |   |   `-- util
|   |   |-- servo
|   |   |   `-- include
|   |   |       `-- goliath
|   |   |-- vision
|   |   |   |-- detectors
|   |   |   |-- include
|   |   |   |   `-- goliath
|   |   |   |-- processors
|   |   |   `-- util
|   |   `-- zmq-messaging
|   |       |-- include
|   |       |   `-- goliath
|   |       `-- src
|   `-- resources
|-- tests
`-- third_party
    |-- lib-dynamixel
    `-- proto
```

### Explanation
**CMake Modules**
```
cmake
`-- modules
```
Contains utitility `.cmake` files used by different `CmakeLists.txt`'s

**Documentation**
```
doc
```
Contains files for generating documentation, like this page. Also logic to automatically update documentation.

**Source**
```
src
```
Contains all written source code.

**Controller**
```
controller
|-- commands
`-- util
```
Contains the controller, explained above. Main entrypoint of the application. The `commands` subdirectory contains all commands and helpers. In the root are all `handles` and handle helpers stored.

**Foundation Module**
```
`-- modules
    foundation
    |-- include
    |   `-- goliath
    `-- src
```
Contains defenitions all other module depends on, like defining a base class for a `handle`.

**GPIO Module**
```
`-- modules
    gpio
    `-- include
        `-- goliath
```
Contains interfacing for the Raspberry Pi's GPIO pins.

**I<sup>2</sup>C Module**
```
`-- modules
    i2c
    |-- handles
    `-- include
        `-- goliath
```
Contains communication with Arduino's using the I<sup>2</sup>C protocol. Used primarily by the motor controller module.

**Motor Controller Module**
```
`-- modules
    motor-controller
    |-- include
    |   `-- goliath
    `-- sketches
```
Contains definitions for controlling the motor Arduino, as well as sketches stored on said Arduino.

**OpenAL Module**
```
`-- modules
    openal
    |-- include
    |   `-- goliath
    `-- util
```
Contains audio utilities that may or may not be used with the dance exercises.

**Servo Module**
```
`-- modules
    servo
    `-- include
       `-- goliath
```
Contains abstractinos for driving and controlling servo's.

**Vision Module**
```
`-- modules
    vision
    |-- detectors
    |-- include
    |   `-- goliath
    |-- processors
    `-- util
```
Contains everything vision and camera related. `detectors` are objects that detect objects / shapes within images and `processors` alter an image in some way.

**ZMQ Messaging Module**
```
`-- modules
    zmq-messaging
    |-- include
    |   `-- goliath
    `-- src
```
Connects the core with the outside world, using Google's protobufs. Dependency of the controller.

**Resources**
```
resources
```
Any other type of file that is not a source, but is needed by the project is stored here.

**Unit Tests**
```
tests
```
Contains unit tests to test a specific module on it's functionallity.

**Third Party**
```
third_party
```
Contains any other code / libraries not defined in the `core` project.

**Dynamixel**
```
`-- third_party
    `-- lib-dynamixel
```
Modified version of the `Dynamixel` library to cater to our needs.

**Protobuf**
```
`-- third_party
    `-- protobuf
```
Contains all protobufs being used by our project(s), they are passed between the broker and the `core` with the ZMQ Messaging Module. Protobuf's are written in a custom language that is cross platform.
