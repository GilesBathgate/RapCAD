Primitive* CGALPrimitive::taper(const CGAL::Scalar& amount)
{
	CGALExplorer explorer(this);
	CGALPrimitive* surface = explorer.getPrimitive();
	if (!surface) return nullptr;

	QList<QList<CGALPolygon*>> baseFaces = explorer.getBaseFaces();
	if (baseFaces.isEmpty()) {
		delete surface;
		return this->copy();
	}

	QList<CGAL::Point3>& pts = surface->points;
	QMap<CGAL::Point3, CGAL::Vector3> movements;

	for (const auto& faces : baseFaces) {
		for (int i = 0; i < faces.size(); ++i) {
			CGALPolygon* pg = faces[i];
			CGAL::Polygon_2<CGAL::Kernel3> polygon;
			const auto& pgPoints = pg->getPoints();
			int n = pgPoints.size();
			if (n > 1 && pgPoints[0] == pgPoints[n-1]) n--;
			for (int j = 0; j < n; ++j) {
				polygon.push_back(CGAL::Point2(pgPoints[j].x(), pgPoints[j].y()));
			}

			if (polygon.is_clockwise_oriented()) {
				polygon.reverse_orientation();
			}

			bool isHole = (i > 0);
			bool shrink = isHole ? (amount > 0.0) : (amount < 0.0);
			CGAL::Scalar abs_amount = amount < 0.0 ? -amount : amount;

			boost::shared_ptr<CGAL::Straight_skeleton_2<CGAL::Kernel3>> ss;
			if (shrink) {
				ss = CGAL::create_interior_straight_skeleton_2(polygon, CGAL::Kernel3());
			} else {
				ss = CGAL::create_exterior_straight_skeleton_2(abs_amount, polygon, CGAL::Kernel3());
			}

			if (!ss) continue;

			typedef CGAL::Straight_skeleton_2<CGAL::Kernel3> Ss;
			CGAL::Polygon_offset_builder_traits_2<CGAL::Kernel3> traits;
			CGAL::Polygon_offset_builder_2<Ss, decltype(traits), CGAL::Polygon_2<CGAL::Kernel3>> ob(*ss, traits);

			for (auto it = ss->halfedges_begin(); it != ss->halfedges_end(); ++it) {
				if (it->is_bisector() && it->opposite()->vertex()->is_contour()) {
					auto cv = it->opposite()->vertex();
					auto be = it;

					auto opt_p = ob.Construct_offset_point(abs_amount, be);
					if (opt_p) {
						CGAL::Point2 p2 = *opt_p;
						CGAL::Vector3 dir(p2.x() - cv->point().x(), p2.y() - cv->point().y(), 0);
						CGAL::Point3 p3(cv->point().x(), cv->point().y(), pgPoints.first().z());
						movements[p3] = dir;
					}
				}
			}
		}
	}

	for (int i = 0; i < pts.size(); ++i) {
		if (movements.contains(pts[i])) {
			pts[i] = pts[i] + movements[pts[i]];
		}
	}

	surface->setType(PrimitiveTypes::Volume);
	surface->setSanitized(false);
	surface->solidify();
	surface->appendChild(this);
	return surface;
}
