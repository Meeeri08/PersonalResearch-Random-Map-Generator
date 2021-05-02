## Random Map Generation

My name is Maria Garrigolas Ledo, student of the [Bachelor's Degree in Video Games Design and Development](https://www.citm.upc.edu/ing/estudis/graus-videojocs/) at UPC CITM. 

This content is generated for the second year's subject Project II, under supervision of lecturer [Ramon Santamaria](https://github.com/raysan5).

### About the research: What are Random Generated Maps?

The procedural generation is the process to generate content algorithmically rather than manual. 

In other words, random generated maps are created by computers instead of persons.

|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/random_map_1.gif" alt="Height Formula" width="600" height="300">|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/random_map_2.gif" alt="Height Formula" width="600" height="300">|
|:---:|:---:|
|Random generated terrain|Random generated dungeon|

### Why do we want random generated maps?
Nowadays, a video game demands several materials to compose complex and large scene, carrying high costs in their content development. Developers use these algorithms to
reduce the amount of time taken to create a game or certain features of it.

Also, from the player perspective, it is more enjoyable to play a diferent map every time. So replayability is also a very important factor.


### Characteristics of a generated random map
#### Why is it good?

- It saves development time and money
- It can save on memory usage
- It increases replayability
- It creates dynamic content
- It creates a large variety of options

#### Why is it bad?
- You may generate an unplayable world
- Worlds can feel repetitive


### When should we use a random generated map?
- Open world or survival games
- Space exploration and trading games.

|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/minecraft_cover.png" alt="Height Formula" width="300" height="500">|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/no_mans_sky.PNG" alt="Height Formula" width="300" height="500">|
|:---:|:---:|
|Minecraft |No man's sky|


### How can we create this algorithm?


The tricky part in procedural generation is not to make things random, but to make them in a consistent way despite it’s randomness. 
There are two types of maps:
|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/outdoor_map.gif" alt="Height Formula" width="600" height="340">|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/indoor_map.gif" alt="Height Formula" width="600" height="340">|
|:---:|:---:|
|Outdoor map |Indoor map|

We will use different procedures to achieve each of them.

#### Dungeon Generation using Binary Space Trees

When making a dungeon you have to face the problem of filling the space with elements in a natural way.

First step we have to do is to divide a plane into two sets recursively. We divide until we can't divide anymore or until we reach a maximum number of spaces.

|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/algorithm_1.png" alt="Height Formula" width="612" height="204">|
|:---:|
|Dividing a square into two recursively|

Next step is to add borders: we got a random division, but we don’t want the rooms to use the whole space, so let’s add a method to cut their borders recursively.
We can achieve that giving a regular or non regular margin between space wqall and room  wall.

|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/algorithm_2.png" alt="Height Formula" width="612" height="204">|
|:---:|
|Making rooms and connecting them using a binary space partition|

The last step is to add corridors, we are going to do this by recursively connecting each node with it's sibling. 

|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/algorithm_5.png" alt="Height Formula" width="507" height="566">|
|:---:|
|Creating corridors|
 
 This should be the final result: 
 
|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/algorithm_7.png" alt="Height Formula" width="498" height="490">|
|:---:|
|Final result|

#### Perlin Noise

<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/noise.png" alt="Height Formula" width="801" height="403">


### TODOs
#### TODO 1: Create a FastNoiseLite object.
```ruby
FastNoiseLite noise;
```
#### TODO 1.2: Set its Noise type to Perlin noise.
```ruby
noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin); 

```
#### TODO 1.3: Set the seed with SetSeed function.
```ruby
noise.SetSeed(seed);
```
#### TODO 1.4: Set the frequency to 0.05.
```ruby
noise.SetFrequency(0.05);

```
Output: Nothing will be shown on screen yet.
#### TODO 2: Store the values generated by Perlin Noise in app->map->height_map.
```ruby
app->map->height_map[x][y] = noise.GetNoise((float)x, (float)y);

```
#### TODO 2.1: Noise must be always between 1 and 0 (use the following formula: (Noise + 1) *0.5).
```ruby
app->map->height_map[x][y] = (noise.GetNoise((float)x, (float)y) +1) * 0.5;

```
Output

|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/todo_2.png" alt="Height Formula" width="500" height="400">|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/todo_2_2.PNG"  alt="Height Formula" width="500" height="400">|
|:---:|:---:|
|TODO 2|TODO 2.1|

#### TODO 3: Draw the different textures into the map.
```ruby
if (value > 0 && value < 0.2) app->render->DrawTexture(app->scene->forestTex,pos.x, pos.y, NULL, scale);
else if (value > 0.2 && value < 0.4) app->render->DrawTexture(app->scene->grassTex, pos.x, pos.y, NULL, scale);
else if (value > 0.4 && value < 0.6)  app->render->DrawTexture(app->scene->sandTex, pos.x, pos.y, NULL, scale);
else if (value > 0.6 && value < 1)  app->render->DrawTexture(app->scene->waterTex, pos.x, pos.y, NULL, scale);
```
Output: The terrain will be shown in screen with its textures. Use [SPACE] to generate new terrains with the generated perlin noises!


|<img src="https://raw.githubusercontent.com/Meeeri08/PersonalResearch-Random-Map-Generator/main/docs/Assets/todo_3.PNG" alt="Height Formula" width="500" height="400">|
|:---:|
|TODO 3|

## Bibliography
Library used: [Fast Portable Noise Library created by Jordan Peck](https://github.com/Auburn/FastNoiseLite)

A video about [2D Terrain Generation using Perlin Noise](https://www.youtube.com/watch?v=jv6YT9pPIHw) published by [Barney](https://github.com/BarneyWhiteman/CodingChallenges)

[Procedural generation applied to a video game level design](https://upcommons.upc.edu/bitstream/handle/2099.1/26632/109439.pdf?sequence=1&isAllowed=y)  written by Albert Carrión Díaz

Addition information to understand [procedural dungeon](https://www.freecodecamp.org/news/how-to-make-your-own-procedural-dungeon-map-generator-using-the-random-walk-algorithm-e0085c8aa9a/)




