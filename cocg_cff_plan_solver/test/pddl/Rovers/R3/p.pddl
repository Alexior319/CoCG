(define (problem roverprob1234) (:domain Rover)
(:objects
        general - lander
        colour high_res low_res - mode
        rover0 rover1 - rover
        rover0store - store

        waypoint0 waypoint1 waypoint2 waypoint3 - waypoint
        camera0 camera1 - camera
        objective1 - objective
        )
(:init

	(visible waypoint1 waypoint0) 
	(visible waypoint2 waypoint0)
	(visible waypoint3 waypoint0)
	(visible waypoint0 waypoint1)

	(visible waypoint0 waypoint2)
	(visible waypoint2 waypoint1)
	(visible waypoint1 waypoint2)

	(visible waypoint0 waypoint3)
	(visible waypoint3 waypoint1)
	(visible waypoint1 waypoint3)
	(visible waypoint3 waypoint2)
	(visible waypoint2 waypoint3)
	(at_lander general waypoint0)

	(channel_free general)
	(at rover0 waypoint3)
	(available rover0)
	(equipped_for_imaging rover0)
	(can_traverse rover0 waypoint3 waypoint0)
	(can_traverse rover0 waypoint0 waypoint3)
	(can_traverse rover0 waypoint3 waypoint1)
	(can_traverse rover0 waypoint1 waypoint3)
	(can_traverse rover0 waypoint1 waypoint2)
	(can_traverse rover0 waypoint2 waypoint1)

	(on_board camera0 rover0)
	(calibration_target camera0 objective1)
	(supports camera0 colour)
	(supports camera0 high_res)
	
	
	(at rover1 waypoint3)
	(available rover1)
	(equipped_for_imaging rover1)
	(can_traverse rover1 waypoint3 waypoint0)
	(can_traverse rover1 waypoint0 waypoint3)
	(can_traverse rover1 waypoint3 waypoint1)
	(can_traverse rover1 waypoint1 waypoint3)
	(can_traverse rover1 waypoint1 waypoint2)
	(can_traverse rover1 waypoint2 waypoint1)
	
    (on_board camera1 rover1)
	(calibration_target camera1 objective1)
	(supports camera1 colour)
	(supports camera1 high_res)

	(unknown (visible_from objective1 waypoint0))
	(unknown (visible_from objective1 waypoint1))

	(oneof
         (visible_from objective1 waypoint0)
         (visible_from objective1 waypoint1)
	)

)

(:goal
(communicated_image_data objective1 high_res)

)
)

