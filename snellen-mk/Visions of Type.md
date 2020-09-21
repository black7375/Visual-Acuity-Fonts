# [Visions of Type](http://mkweb.bcgsc.ca/typography/eyecharts.mhtml)

| ![snellenmk.poster.01](https://user-images.githubusercontent.com/25581533/93721966-e8e71500-fb82-11ea-8613-8f7563c0ba5a.png) | ![snellenmk.poster.03](https://user-images.githubusercontent.com/25581533/93723397-ce666900-fb8d-11ea-986b-c5a1145d0eff.png) | ![snellenmk.poster.02](https://user-images.githubusercontent.com/25581533/93721968-eb496f00-fb82-11ea-97e1-30ec439b792f.png) |
|------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------|
| ▲ SnellenMK optotype font. Uppercase, lowercase and symbols to test your eyes.                                              | ▲ SnellenMK optotype font. Uppercase, lowercase and symbols to test your eyes.                                              | ▲ SnellenMK optotype font. Uppercase, lowercase and symbols to test your eyes.                                              |

# Eyes On the Universe
These typographical posters are designed after the style of the [Snellen Chart](https://en.wikipedia.org/wiki/Snellen_chart), which is one of the kinds of eye charts used to measure visual acuity.

If you love looking, seeing and the universe, these posters are for you. They are [available for purchase](https://fineartamerica.com/profiles/martin-krzywinski?tab=artworkgalleries&artworkgalleryid=714111).

## Optotypes
Symbols on such charts are known as optotypes. Fonts by Andrew Howlett exist whose glyphs conform to the properties of optotypes: [Snellen font](http://mkweb.bcgsc.ca/typography/fonts/snellen-font.zip) and [Sloan font](http://mkweb.bcgsc.ca/typography/fonts/sloan-font.zip). However, some of the characters in the Snellen font file are a little oddly shaped—I provide my [redesign of the Snellen font](http://mkweb.bcgsc.ca/snellen-optotype-font/) in which the glyphs are more consistent (see below). Lowercase characters are not available.

For the posters here, I've used either my [redesigned Snellen font](http://mkweb.bcgsc.ca/snellen-optotype-font/) or [Monotype's Rockwell](https://www.fonts.com/font/monotype/rockwell), with minor stroke and kerning adjustments in places. Some symbols, such as on the math chart, were designed by hand.

The numbers on the left side of the posters (e.g. 20/30) are a measure of visual acuity. The numbers on the right provide information about what is shown on the line (e.g. abundance of elements).

## Snellen chart design
The charts are designed to be viewed at a distance of 6 meters (20 feet). At this distance, ability to resolve a letter tha subtends 5 minute of arc corersponds to 6/6 (or 20/20) [visual acuity](https://en.wikipedia.org/wiki/Visual_acuity). This corresponds to a letter size of

![(2π/360) × (5/60) × 6 = 8.727mm = 24.74pt](https://render.githubusercontent.com/render/math?math=\huge%20\frac{2\pi}{360}%20\times%20{5%20\over%2060}%20\times%206%20=%208.727%20mm%20=%2024.74%20pt)
``` latex
\frac{2\pi}{360} \times {5 \over 60} \times 6 = 8.727 mm = 24.74 pt
```

The Snellen optotypes are designed on a 5 × 5 grid and have a [fascinating history](https://gizmodo.com/examining-the-typographic-history-of-eye-charts-1732823179). For design, [Rockwell](https://www.fonts.com/font/monotype/rockwell) and [Lubalin Graph](https://www.linotype.com/1488211/itc-lubalin-graph-family.html) can be used to approximate Snellen, though these fonts lack the grid structure of the optotypes.

![optotypes](https://user-images.githubusercontent.com/25581533/93724093-ac6fe500-fb93-11ea-921a-8defb47c8745.png)
▲ Snellen optotypes are designed on a 5 × 5 grid. At a viewing distance of 6 m (20 ft) each letter on the 6/6 (or 20/20) acuity line must be 8.727 mm (24.47 pt). The optotypes are compared to the characters from the Rockwell Bold font, which is a mediocre approximation.

### snellen optotype font
My redesign of Andrew Howlett's Snellen optotype font. Read about [redesign process](http://mkweb.bcgsc.ca/snellen-optotype-font/)—which reinterprets some of the characters and adds lowercase.

You can [download both versions of the font](http://mkweb.bcgsc.ca/snellen-optotype-font/).

![snellen-optotype-font-v07.1-01](https://user-images.githubusercontent.com/25581533/93723138-93633600-fb8b-11ea-86a0-c4922a404394.png)
▲ Snellen optotypes are designed on a 5 × 5 grid. At a viewing distance of 6 m (20 ft) each letter on the 6/6 (or 20/20) acuity line must be 8.727 mm (24.47 pt)

## The Posters

### Conventional Snellen charts
These Snellen charts include acuity lines from 20/200 to 20/10.

The charts should be printed at a physical size of 16" × 24" (1150 pt × 1725 pt. At this size, the characters on the 20/20 line subtend 5 minutes of arc when viewed at 6 meters (20 feet), which is the technical specification of the Snellen chart.

When the charts are printed at this size, the two horizontal lines below the 20/30 and 20/20 lines are exactly 8" (576 pt) long. These length markers are my own addition.

If the chart is printed at any other size, the viewing distance changes. To compute the correct viewing distance, d, measure the length of these lines, L (in inches) and use

![d = 6 × L/8](https://render.githubusercontent.com/render/math?math=\huge%20d%20=%206%20\times%20L%20/%208)
``` latex
d = 6 \times L / 8
```

For example, if I print this chart to fit onto an 8.5" × 11" page, these lines are 3.47". Thus, my smaller chart should be viewed from 6×3.47/8=2.60m(8.53 ft).

Numbers on the left provide visual acuity in feet. Numbers on the right show the denominator of the acuity in feet and its equivalent in meters, rounded to the nearest integer.

The order of the 61 characters on the charts has been limit uniformity and avoid easily perceived patterns—especially in the case of the genetic sequence Snellen. These restrictions (e.g. limit in the number of repeated n-grams) apply across linebreaks.

#### 9 character Snellen
This is the canonical Snellen chart, using the 9 original characters.

```
E FP LDO CETD ZOFEL DCZTFP PFLOZDE OZPCELTD TLEFDCOP EDOPTFLC LTCZOEPF FODLPZCT
```

1. no more than 8 instances of any character and no fewer than 6
2. no double characters (e.g. PP does not occur)
3. no more than 2 repeats of any 2-gram (e.g. LT ... LT ... LT does not occur)
4. all 3-grams are unique (e.g. LDO does not repeat)
5. no identical adjacent characters across lines within a distance of one positions.
6. for a given line, the characters at the same position in the previous 6 lines are all different.

![snellen-chart-efp-01](https://user-images.githubusercontent.com/25581533/93724407-4f296300-fb96-11ea-84f1-f89c7355105b.png)
▲ A technically accurate Snellen chart using traditional 9 characters C D E F L O P T Z rendered as optotypes. Print at 16 in × 24 in. ([BUY ARTWORK](https://fineartamerica.com/featured/snellen-chart-9-character-martin-krzywinski.html))

#### 26 character Snellen
This chart uses all the letters of the alphabet and is typset using my Snellen font redesign.

1. all letters of the alphabet are used
2. no more than 3 instances of any character
3. no double characters (e.g. PP does not occur)
4. all n-grams (n = 2, 3, ...) are unique
5. on a given line, all characters are unique
6. no identical adjacent characters across lines within a distance of 8 positions.
7. for a given line, the characters at the same position in all other lines are all different.

```
E FP NBJ GCHQ RKVNX PZLSAY IMEXDBU CYRAVQGH LWKPIJZO XUBHRFEV JTDIGSYZ QFWLMUKA
```

![snellen-chart-alphabet-01](https://user-images.githubusercontent.com/25581533/93724818-e7751700-fb99-11ea-83be-9b689da13951.png)
▲ A technically accurate Snellen chart using all 26 letters of the alphabet rendered as optotypes. All n-grams are unique. Print at 16 in × 24 in. ([BUY ARTWORK](https://fineartamerica.com/featured/snellen-chart-full-alphabet-martin-krzywinski.html))

#### Genomic sequence Snellen
Since I work in a genome center, the one below is the one we'd use. Thanks to Dr. Nüket Bilgen for suggesting that the chart start with ATG (start codon) and end with one of the stop codons (TAG, TGA, but not TAA since no two adjoining characters can be the same).

1. no more than 19 instances of any character and no fewer than 15
2. no double characters (e.g. AA does not occur)
3. no more than 7 repeats of any 2-gram
4. no more than 4 repeats of any 3-gram
5. no more than 2 repeats of any 4-gram or 5-gram
6. for a given line, the characters at the same position in the previous 2 lines are different
7. chart starts with start codon ATG
8. chart ends with stop codon TAG, which appears only once; the other two stop codons (TGA, TAA) do not appear on the chart

```
A TG CAT ATCG GCATA CGTCTG TACAGAC GTGTACGA CGAGCTAT ACTCTGTG GTCAGAGC CGAGATAG
```

![snellen-chart-genetic-sequence-01](https://user-images.githubusercontent.com/25581533/93724849-3b7ffb80-fb9a-11ea-948e-210085241d37.png)
▲ A technically accurate Snellen chart using four genetic bases A T G C rendered as optotypes. The chart begins with the start codon ATG and ends in the stop codon TGA, which appears only once in the chart. Print at 16 in × 24 in. ([BUY ARTWORK](https://fineartamerica.com/featured/snellen-chart-genetic-sequence-martin-krzywinski.html))

The best alignments of this chart's sequence are to fungus (*Leptosphaeria maculans lepidii*, 35/42, 83%) and a tapeworm (*Diphyllobothrium latum*, 24/26, 92%). Thanks to Lorraine May for this observation!

#### nautical flags Snellen
Charts ahoy!

```
Z KE CHG XVRM YTWUS JQFINB EZAOXLD NHKVCUGF SWRMIAZP DBTOJYXE FZHLNUKA IVGMYCWR
```

![snellen-chart-nautical-flags](https://user-images.githubusercontent.com/25581533/93722374-e2a66800-fb85-11ea-8ee6-8b79e60eda54.png)
▲ A technically accurate Snellen chart using the nautical flag alphabet rendered as optotypes. Print at 16 in × 24 in. ([BUY ARTWORK](https://fineartamerica.com/featured/snellen-chart-nautical-flags-martin-krzywinski.html))

The flag alphabet has been designed to match, as closely as possible, to the style of the Snellen optotypes. In some cases this required that the geometry of the flag had to be adjusted—this may upset the purists and cause havoc on the waterways.

Proportions of colors has been adjusted in some flags to fit symmetrically into the 5 × 5 optotype grid. The checker of N is now a 5 × 5 grid. The number of stripes in Y has been reduced—the width of each stripe is now 20% of the width of the flag. Proportions in C, D, J, R, S, T, W and X have been adjusted so that color strips are a multiple of 20% of the width of the flag. The cross in M and V matches the X used in the Snellen font. 

![nautical-flag-optotypes-01](https://user-images.githubusercontent.com/25581533/93725004-3ff8e400-fb9b-11ea-9370-ac56cadb9852.png)
▲ Snellen optotypes for the nautical flag alphabet.

### Eyes on the elements
Elements are sorted in order of abundance. The numbers on the left show the max and min −log10 abundance of the elements listed on a given line. For example, 3.0/3.3 for the "N Si Mg S" line in the abundance of elements in the universe indicates that abundance of N is 0.001 and of S is 0.0005.

You can download my tidy plain-text [table of abundance of elements in the universe](http://mkweb.bcgsc.ca/typography/elements.parsed.universe.txt) ([original source](https://periodictable.com/Properties/A/UniverseAbundance.v.log.html), 83 elements) and [table of abundance of elements in the body](http://mkweb.bcgsc.ca/typography/elements.parsed.body.txt) ([original source](https://en.wikipedia.org/wiki/Composition_of_the_human_body), 60 elements). These have been parsed from the original sources and give the −log10 abundance for various elements.

![snellen-chart-elements-universe](https://user-images.githubusercontent.com/25581533/93725109-0e344d00-fb9c-11ea-814a-fdf4ac45db7b.png)
▲ Snellen Chart of abundance of the elements in the universe. ([BUY ARTWORK](https://fineartamerica.com/featured/snellen-chart-chemical-abundance-in-universe-martin-krzywinski.html))

![snellen-chart-elements-body](https://user-images.githubusercontent.com/25581533/93725118-14c2c480-fb9c-11ea-8920-4866bbe75ff4.png)
▲ Snellen Chart of abundance of the elements in the human body. ([BUY ARTWORK](https://fineartamerica.com/featured/snellen-chart-chemical-abundance-in-human-body-martin-krzywinski.html))

### Eyes on physical constants
44 of the most interesting physical constants ranging from the very large (Planck temperature Tp=1.4×1032K) to the very small (cosmological constant Λ=1.19×10−52m−2). You can [download the table of constants and their values](http://mkweb.bcgsc.ca/typography/physical.constants.txt).

![snellen-chart-physical-constants](https://user-images.githubusercontent.com/25581533/93725231-f4dfd080-fb9c-11ea-856f-49928fc96fed.png)
▲ Snellen Chart of physical constants. ([BUY ARTWORK](https://fineartamerica.com/featured/snellen-chart-physical-constants-martin-krzywinski.html))

### Eyes on mathematical symbols
44 intriguing and perhaps mysterious mathematical symbols ranging from common equality = to the esoteric normal subgroup ◃.

![snellen-chart-math](https://user-images.githubusercontent.com/25581533/93725261-2ce71380-fb9d-11ea-881c-f3080170a7e5.png)
▲ Snellen Chart of mathematical operators and symbols. ([BUY ARTWORK](https://fineartamerica.com/featured/snellen-chart-mathematical-symbols-martin-krzywinski.html))

### Where's the chart?
The chart is the visual form of a rhetorical question. The letter layout here is the same as in the [canonical Snellen chart](https://en.wikipedia.org/wiki/Snellen_chart), which is limited to the 10 [Sloan letters](https://en.wikipedia.org/wiki/Sloan_letters) C, D, E, F, L, N, O, P, T, Z.

![snellen-chart-braille](https://user-images.githubusercontent.com/25581533/93725339-0a092f00-fb9e-11ea-8881-767a9ed1b06b.png)
▲ Snellen Chart typeset in Braille. ([BUY ARTWORK](https://fineartamerica.com/featured/snellen-chart-braille-martin-krzywinski.html))

![snellen-chart-braille-2](https://user-images.githubusercontent.com/25581533/93725341-0bd2f280-fb9e-11ea-9e4b-69b8eea709fa.png)
▲ Snellen Chart typeset in Braille. Variant #2. ([BUY ARTWORK](https://fineartamerica.com/featured/1-snellen-chart-braille-martin-krzywinski.html))

![snellen-chart-braille-3](https://user-images.githubusercontent.com/25581533/93725342-0d041f80-fb9e-11ea-9269-e5f869ffa485.png)
▲ Snellen Chart typeset in Braille. Variant #3. ([BUY ARTWORK](https://fineartamerica.com/featured/2-snellen-chart-braille-martin-krzywinski.html))

# LICENSE
© 1999–2020 [Martin Krzywinski](http://mkweb.bcgsc.ca/) | [contact](martink@bcgsc.ca)
