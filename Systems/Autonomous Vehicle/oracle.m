function ideal (Bread1, Bread2)

    global bread;
    
    while Bread1.y <= LaserScannerCoordinate

        Bread2.x = Bread1.x;
        Bread2.y = Bread1.y;
        Bread2.z = Bread1.z + bread.fillingsHeight + (bread.height/2);
        Bread2.theta = Bread1.theta;
    
    end

end