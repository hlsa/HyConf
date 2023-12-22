translate([0,0,0.05]) {
    cube([1,1,0.1],true);
    
    translate([0,0,0.45])
    cube([0.02,0.02,1],true);
    
    translate([0.14,0,0.96])
    cube([0.3,0.02,0.02],true);
    
    translate([0.14,0,0.98])
    cube([0.3,0.02,0.02],true);
    
    translate([0.30,0,0.84])
    cube([0.02,0.02,0.3],true);
    
    translate([0.32,0,0.84])
    cube([0.02,0.02,0.3],true);
    
    translate([0.32,0,0.6875])
    cube([0.1,0.1,0.005],true);
    
    translate([0.32,0,0.685])
    color("blue",1)
    sphere(0.005, $fn=100);
}


