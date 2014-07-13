
function [chord, beta, gamma, delta, epsilon, distance] =  GetDistance(ArmLength, StartAngle, StartDistance, alpha)

    alpha =  -alpha;
    angle = 0:850;
    delta = (180.0 - alpha);
    chord = zeros(size(angle)); % chord in further arm location
    beta = zeros(size(angle));
    gamma = zeros(size(angle)); 
    epsilon = zeros(size(angle)); 
    distance = zeros(size(angle)); 

    for i = 1:length(angle)   
        
        %angle(i) = abs(angle(i) - 360); % for clockwise turn     
       
        % for more cycles
        if(angle(i) > 720)
            angle(i) = angle(i) - 720;
        end
        if(angle(i) > 360)
            angle(i) = angle(i) - 360;
        end
       
        
      chord(i) = GetChord(ArmLength,StartAngle, angle(i));  
      beta(i) = abs( iif(angle(i) < StartAngle, 360 + angle(i), angle(i)) - StartAngle);
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










 
