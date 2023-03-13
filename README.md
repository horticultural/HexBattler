# HexBattler

Project is made with Unreal Engine 5.1.1

Clone the repo, right click the .uproject and generate visual studio project files.
Open the .sln and compile from there.

In the demo level you can press play and manouvre the camera to see the whole board.
Though I like to go into Top View mode to see the birds eye view.

By clicking on the BP_HexGrid1 Actor in the outliner, you can set the Hex grid width, heigh, seed number and quantity of red and blue team units. Other details should not be changed.

If you open up the BP_Battler blueprint, you can change the class default details such as Tile Range, starting health, Turns Per Attack, Turns Per Move, Damage Range.

The fights are deterministic in that they always have the same result. Changing the seed number will affect the result.

Regarding pathing:
I only made a simple pathing algorithm which essentially just makes them match on the x axis, followed by the y axis. There's no tile occupation check, so multiple battlers can be on the same tile.
Putting in a proper A* algorithm would have taken a while, and I probably would have wanted to change my hex coords to axial or similar, rather than the current square offset based system.

Otherwise the project fills all other requirements. I look forward to hearing feedback on everything!
