function chord = GetChord(ArmLength, StartAngle, angle)

        chord = abs( (ArmLength*sin(abs( iif(angle < StartAngle, 360 + angle, angle) - StartAngle) *pi / 180)) / sin((180 - abs(iif(angle < StartAngle, 360 + angle, angle) - StartAngle))*pi / 360));
     
        if chord > 2*ArmLength 
            chord = 2*ArmLength;
        end
        
        if chord < 0
            chord = 0;
        end
end



