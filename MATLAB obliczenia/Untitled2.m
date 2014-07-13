clc
ArmLength = 100;
StartAngle = 180;
angle = 0:0.1:360;

chord = zeros(size(angle));

for i = 1:length(angle) 
    
  chord(i) = GetChord(ArmLength,StartAngle, angle(i) );
  if chord(i) > 200
      angle(i)
  end
end

plot(chord);
max(chord)