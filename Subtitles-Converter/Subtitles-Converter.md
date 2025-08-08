# Lab Assignment №2 – Subtitles Processing

## Tools and Requirements

* **Language**: C++
* **Standard**: C++23
* **Testing**: All implemented classes and functions must be covered with unit tests.

---

## Task

Implement a class hierarchy for handling different subtitle formats:

* A **base class** `Subtitle`.
* Derived classes for each supported subtitle format.

The program must be able to **read** and **write** subtitles in supported formats.
The format is determined by **output file extension**, not input file extension.

---

## Supported Formats

| Format                         | Extension(s)    | Requirements                                                                                                                                              |
| ------------------------------ | --------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **SubRip**                     | `.srt`          | Full support                                                                                                                                              |
| **SAMI**                       | `.smi`          | Global styles (`<STYLE>`) support is optional                                                                                                             |
| **Advanced Sub Station Alpha** | `.ass`, `.ssa`  | Support `[Script Info]`, `[Styles]`, `[Events] Dialogue` sections; ignore `Effect` in `Format`, encodings (`\fe`), active transformations (`\move`, etc.) |
| **One additional format**      | *any extension* | Implement at your choice                                                                                                                                  |

---

## Functional Requirements for Subtitle Classes

Each subtitle class (`Subtitle` base + derived) must implement methods to:

1. **Remove all formatting styles**
   Remove styles such as bold, italic, font color, etc.

2. **Add a default style**
   Apply a basic default style to all subtitles.

3. **Apply a time shift to all cues**

   * The time delta must be adjustable.
   * The shift can be applied to:

     * **Start & end times**
     * **Start times only**
     * **End times only**

4. **Find overlapping cues**

   * Detect collisions (time overlaps) between cues.
   * Return a **new object** containing only overlapping cues.

---

### Example of overlapping subtitles:

```
262
00:14:59.275 --> 00:15:02.570
That said, I'm going to continue
to keep an eye on Rin Okumura.

263
00:15:01.568 --> 00:15:06.323
So please find a position for me
at the Japan Branch.
```

---

## Program Requirements (`main.cpp`)

The program must accept two arguments:

```
<in_file> <out_file>
```

* **Input file**: File to read (extension does not determine format).
* **Output file**: File to write (extension determines format).

**Example:**

```bash
in.srt out.smi
```

---

### Example Conversion

**Input (`in.srt`)**

```
1
00:00:32,940 --> 00:00:36,460
1st block of Taito district, Shin-Ueno line.

2
00:00:36,560 --> 00:00:39,060
A significant excess of Psychopass indicators was registered on the motorway overpass.

3
00:00:39,220 --> 00:00:41,560
The duty team of inspectors and performers
```

**Output (`out.smi`)**

```xml
<SAMI>
<BODY>
<SYNC Start=32940 End=36460><P>1st block of Taito district, Shin-Ueno line.</P></SYNC>
<SYNC Start=36560 End=39060><P>A significant excess of Psychopass indicators was registered on the motorway overpass.</P></SYNC>
<SYNC Start=39220 End=41560><P>The duty team of inspectors and performers</P></SYNC>
</BODY>
</SAMI>
```
