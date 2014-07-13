
function distance = GetDistance(ArmLength, StartAngle, StartDistance, alpha)
    angle = 0:360;
    delta = abs(180.0 - alpha);
    chord = zeros(size(angle)); % chord in further arm location
    beta = zeros(size(angle));
    gamma = zeros(size(angle)); 
    epsilon = zeros(size(angle)); 
    distance = zeros(size(angle)); 

    for i = 1:length(angle)     
      chord(i) = GetChord(ArmLength,StartAngle, angle(i));  
      beta(i) = abs(StartAngle - angle(i));  %abs?   
      gamma(i) =  (180 - beta(i) ) / 2; 
      epsilon(i) = 360 - gamma(i) - delta; 

      distance(i) = sqrt(power(chord(i), 2) + power(StartDistance, 2) - 2 * chord(i)*StartDistance*cos(deg2rad(epsilon(i))));
    end

    figure('Name', 'Chord')
    plot(chord);

    figure('Name', 'Beta');
    plot(beta);

    figure('Name', 'Gamma')
    plot(gamma);

    figure('Name', 'Epsilon')
    plot(epsilon);

    figure('Name', 'Distance')
    plot(distance);

end










 
